const transaction = require("../../models/crud/transaction");
const customer = require("../../models/crud/customer");
const account = require("../../models/crud/account");
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

router.get("/transactions", (req, res) => {
    // Limit offset to 0 or higher (Also substract 1 for prev check)
    const offset = Math.max(Number(butil.nshcl(req.query.offset, 0)) - 1, 0);
    // Limit amount to 1-30
    const itemCount = Math.min(Math.max(Number(butil.nshcl(req.query.count, 10)), 1), 30);

    transaction.fromOffset(
        req.token.accountId,
        offset,
        itemCount + 1 // Add 1 for next check
    )
    .then(async results => {
        if(results.length < 1) {
            res.json({ transactions: results, count: 0, nextOffset: 0 });
            return;
        }

        const hasPrev = results[0].transactionId < offset;
        const hasNext = results.length - hasPrev > itemCount;
        const retData = results.slice(hasPrev, results.length - hasNext);

        res.json({
            transactions: retData,
            count: retData.length,
            nextOffset: results[results.length - 1].transactionId + 1,
            hasPrev,
            hasNext
        });
    })
    .catch(error => butil.handleQueryError(error, res));
});

module.exports = router;