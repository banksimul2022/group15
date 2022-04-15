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

module.exports = router;