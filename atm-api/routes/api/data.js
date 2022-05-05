const transaction = require("../../models/crud/transaction");
const customer = require("../../models/crud/customer");
const account = require("../../models/crud/account");
const card = require("../../models/crud/card");
const errors = require("../../errors");
const butil = require("../../util");
const { posix } = require ("path");

const router = require("express").Router();

router.get("/info", (req, res) => {
    customer.getById(req.token.customerId)
        .then(async custResult => {
            const accResult = await account.getById(req.token.accountId);
            const cardResult = await card.getByCardNumber(req.token.card_number);

            res.json({
                fName: custResult[0].firstName,
                lName: custResult[0].lastName,
                accountNumber: accResult[0].accountNumber,
                credit: Boolean(cardResult[0].credit),
                maxCredit: accResult["maxCredit"],
                profile: custResult[0].profile === null || custResult[0].profile === undefined ? "/static/image/unknown_profile.png" : posix.join("/static/image/profile", custResult[0].profile)
            });
        })
        .catch(error => butil.handleQueryError(error, res));
});

router.get("/balance", (req, res) => {
    account.getById(req.token.accountId)
        .then(async accResult => {
            res.json({
                balance: accResult[0].balance,
                credit: accResult[0].credit
            });
        })
        .catch(error => butil.handleQueryError(error, res));
});

router.get("/transactions/:mode", (req, res) => {
    if(!["back", "forward", "latest"].includes(req.params.mode)) {
        throw new errors.PublicAPIError("Invalid mode type of " + req.params.mode, errors.codes.ERR_INVALID_PARAM, 400);
    }

    // Limit offset to 0 or higher
    const offset = Math.max(Number(butil.nshcl(req.query.offset, 0)), 0);
    // Limit amount to 1-30
    const itemCount = Math.min(Math.max(Number(butil.nshcl(req.query.count, 10)), 1), 30);

    if(req.params.mode === "latest") {
        transaction.latestTransactions(req.token.accountId, itemCount)
            .then(results => {
                res.json({
                    transactions: results,
                    count: results.length
                });
            })
            .catch(error => butil.handleQueryError(error, res));

        return;
    }

    const descent = req.params.mode === "back";

    (descent ? transaction.dscFromOffset : transaction.ascFromOffset)(
        req.token.accountId,
        offset,
        itemCount + 1 // Add 1 for forward/back check
    )
    .then(async results => {
        if(results.length < 1) {
            const queryRes = descent ?
                (await transaction.ascFromOffset(req.token.accountId, offset, itemCount)) :
                (await transaction.dscFromOffset(req.token.accountId, offset, itemCount));

            res.json({
                transactions: results,
                count: 0,
                prevOffset: queryRes.length < 1 ? 0 : queryRes[0].transactionId,
                nextOffset: queryRes.length < 1 ? 0 : queryRes[queryRes.length - 1].transactionId,
                hasPrev: !descent,
                hasNext: descent
            });
            return;
        }

        let hasPrev, hasNext;

        if(descent) {
            hasPrev = results.length > itemCount;
            hasNext = (await transaction.ascFromOffset(req.token.accountId, results[results.length - 1].transactionId, 1)).length > 0;
        } else {
            hasNext = results.length > itemCount;
            hasPrev = (await transaction.dscFromOffset(req.token.accountId, results[0].transactionId, 1)).length > 0;
        }

        const retData = results.slice(descent && hasPrev, results.length - (!descent && hasNext));

        res.json({
            transactions: retData,
            count: retData.length,
            prevOffset: retData[0].transactionId,
            nextOffset: retData[retData.length - 1].transactionId,
            hasPrev,
            hasNext
        });
    })
    .catch(error => butil.handleQueryError(error, res));
});

router.post("/withdraw", (req, res) => {
    account.getById(req.token.accountId)
        .then(async results => {
            const sum = Number(req.body.sum);
            const useCredit = typeof(req.body.credit) === "string" ? req.body.credit === "true" : Boolean(req.body.credit);

            if(useCredit) {
                const cardResult = await card.getByCardNumber(req.token.card_number);

                if(!cardResult[0].credit) {
                    throw new errors.PublicAPIError("the used card doesn't support the credit feature", errors.codes.ERR_CREDIT_NOT_SUPPORTED, 400);
                }
            }

            if(isNaN(sum) || sum < 0.01) {
                throw new errors.PublicAPIError("sum not set or invalid", errors.codes.ERR_INVALID_SUM, 400);
            }

            const accRes = results[0];

            if(useCredit) {
                if(accRes["credit"] + sum > accRes["maxCredit"]) {
                    throw new errors.PublicAPIError("Not enough credit available", errors.codes.ERR_INSUFFICIENT_FUNDS, 200);
                }

                accRes["credit"] += sum;
            } else {
                if(accRes.balance < sum) {
                    throw new errors.PublicAPIError("Insufficient funds", errors.codes.ERR_INSUFFICIENT_FUNDS, 200);
                }

                accRes["balance"] -= sum;
            }

            await transaction.add({
                accountId: req.token.accountId,
                timestamp: new Date(),
                accountNumber: accRes["accountNumber"],
                type: useCredit ? "CREDIT_WITHDRAW" : "WITHDRAW",
                sum,
                cardNumber: req.token.card_number
            });

            res.json(await account.update(req.token.accountId, accRes));
        })
        .catch(error => butil.handleQueryError(error, res));
});

router.post("/deposit", (req, res) => {
    account.getById(req.token.accountId)
        .then(async results => {
            const sum = Number(req.body.sum);

            if(isNaN(sum) || sum < 0.01) {
                throw new errors.PublicAPIError("sum not set or invalid", errors.codes.ERR_INVALID_SUM, 400);
            }

            const accRes = results[0];
            accRes["balance"] += sum;

            await transaction.add({
                accountId: req.token.accountId,
                timestamp: new Date(),
                accountNumber: accRes["accountNumber"],
                type: "DEPOSIT",
                sum,
                cardNumber: req.token.card_number
            });

            res.json(await account.update(req.token.accountId, accRes));
        })
        .catch(error => butil.handleQueryError(error, res));
});

router.post("/transfer", (req, res) => {
    account.getById(req.token.accountId)
        .then(async fromResult => {
            const sum = Number(req.body.sum);

            if(isNaN(sum) || sum < 0.01) {
                throw new errors.PublicAPIError("sum not set or invalid", errors.codes.ERR_INVALID_SUM, 400);
            }

            const fromAccount = fromResult[0];

            if(fromAccount.balance < sum) {
                throw new errors.PublicAPIError("Insufficient funds", errors.codes.ERR_INSUFFICIENT_FUNDS, 200);
            }

            let toResult = await account.getByNumber(req.body.accountNumber);

            if(toResult.length < 1) {
                throw new errors.PublicAPIError("Unknown target account", errors.codes.ERR_UNKNOWN_ACCOUNT, 400);
            }

            const toAccount = toResult[0];

            fromAccount["balance"] -= sum;
            toAccount["balance"] += sum;

            const date = new Date();

            await transaction.add({
                accountId: fromAccount.accountId,
                timestamp: date,
                accountNumber: toAccount["accountNumber"],
                type: "TRANSFER_TO",
                sum,
                cardNumber: req.token.card_number
            });

            await transaction.add({
                accountId: toAccount.accountId,
                timestamp: date,
                accountNumber: fromAccount["accountNumber"],
                type: "TRANSFER_FROM",
                sum,
                cardNumber: null
            });

            await account.update(toAccount.accountId, toAccount);
            res.json(await account.update(fromAccount.accountId, fromAccount));
        })
        .catch(error => butil.handleQueryError(error, res))
});

router.post("/changePin", (req, res) => {
    card.getByCardNumber(req.token.card_number)
        .then(async cardResult => {
            const cardDat = cardResult[0];

            if(typeof(req.body.newPin) !== "string" || req.body.newPin.length !== 4) {
                throw new errors.PublicAPIError("newPin invalid", errors.codes.ERR_NEW_PIN_INVALID, 400);
            }

            await butil.checkPin(req.body.pin, cardDat);

            if(req.body.newPin === req.body.pin) {
                throw new errors.PublicAPIError("newPin can't be the same as the current pin", errors.codes.ERR_SAME_PIN, 400);
            }

            cardDat["pin"] = req.body.newPin;

            res.json(await card.update(cardDat["cardId"], cardDat, false));
        })
        .catch(error => butil.handleQueryError(error, res));
});

module.exports = router;
