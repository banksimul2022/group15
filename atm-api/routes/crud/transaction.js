const express = require("express");
const router = express.Router();
const customer_has_account = require("../../models/crud/customer_has_account");
const transaction = require("../../models/crud/transaction");
const permissions = require("../../permissions");
const butil = require("../../util");

const permCheck = async (req) => {
    const result = await transaction.getById(req.params.id);

    // Customer is not linked to transaction if accId is null or no results were found for query
    if(result.length < 1 || await customer_has_account.getById(req.token.customerId, result[0].accountId).length < 1)
        butil.throwIfDenied(permissions.crud.all.account, req.token);

    return result;
};

router.get("/:id?", (req, res) => {
    if (req.params.id) {
        permCheck(req)
            .then(async result => res.json(result))
            .catch(error => butil.handleQueryError(error, res));
    } else {
        transaction.getAll()
            .then(result => res.json(result))
            .catch(error => butil.handleQueryError(error, res));
    }
});

router.post("/", (req, res) => {
    transaction.add(req.body)
        .then(result => res.json(result))
        .catch(error => butil.handleQueryError(error, res));
});

router.delete("/:id", (req, res) => {
    permCheck(req)
        .then(async _ => res.json(await transaction.delete(req.params.id)))
        .catch(error => butil.handleQueryError(error, res));
});

router.put("/:id", (req, res) => {
    permCheck(req)
        .then(async _ => res.json(await transaction.update(req.params.id, req.body)))
        .catch(error => butil.handleQueryError(error, res));
});

module.exports = router;
