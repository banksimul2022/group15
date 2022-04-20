const transaction = require("../../models/crud/transaction");
const customer = require("../../models/crud/customer");
const account = require("../../models/crud/account");
const errors = require("../../errors");
const butil = require("../../util");

const router = require("express").Router();

router.get("/info", (req, res) => {
    customer.getById(req.token.customerId)
        .then(async custResult => {
            const accResult = await account.getById(req.token.accountId);

            res.json({
                fName: custResult[0].firstName,
                lName: custResult[0].lastName,
                accountNumber: accResult[0].accountNumber
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

router.get("/transactions/:direction", (req, res) => {
    if(req.params.direction !== "back" && req.params.direction !== "forward") {
        throw new errors.PublicAPIError("Invalid direction type of " + req.params.direction, errors.codes.ERR_INVALID_PARAM, 400);
    }

    const descent = req.params.direction === "back";

    // Limit offset to 0 or higher
    const offset = Math.max(Number(butil.nshcl(req.query.offset, 0)), 0);
    // Limit amount to 1-30
    const itemCount = Math.min(Math.max(Number(butil.nshcl(req.query.count, 10)), 1), 30);

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

            if(isNaN(sum) || sum < 0.01) {
                throw new errors.PublicAPIError("sum not set or invalid", errors.codes.ERR_INVALID_SUM, 400);
            }

            const accRes = results[0];

            if(accRes.balance < sum) {
                throw new errors.PublicAPIError("Insufficient funds", errors.codes.ERR_INSUFFICIENT_FUNDS, 200);
            }

            accRes["balance"] -= sum;

            await transaction.add({
                accountId: req.token.accountId,
                timestamp: new Date(),
                toAccount: null,
                type: "WITHDRAW",
                sum,
                cardNumber: req.token.cardNumber
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
                toAccount: accRes["accountNumber"],
                type: "DEPOSIT",
                sum,
                cardNumber: req.token.cardNumber
            });

            res.json(await account.update(req.token.accountId, accRes));
        })
        .catch(error => butil.handleQueryError(error, res));
});

module.exports = router;