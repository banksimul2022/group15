const express = require("express");
const router = express.Router();
const permissions = require("../../permissions");
const customer_has_account = require("../../models/crud/customer_has_account");
const account = require("../../models/crud/account");
const butil = require("../../util");

const permCheck = async (req) => {
    // Customer is not linked to account if no results were found
    if((await customer_has_account.getById(req.token.customerId, req.params.id)).length < 1)
        butil.throwIfDenied(permissions.crud.all.account, req.token);
};

router.get("/:id?", (req, res) => {
    if(req.params.id) {
        permCheck(req)
            .then(async () => res.json(await account.getById(req.params.id)))
            .catch(error => butil.handleQueryError(error, res));
    } else {
        account.getAll()
            .then(result => res.json(result))
            .catch(error => butil.handleQueryError(error, res));
    }
});

router.post("/", (req, res) => {
    account.add(req.body)
        .then(result => res.json(result))
        .catch(error => butil.handleQueryError(error, res));
});

router.delete("/:id", (req, res) => {
    permCheck(req)
        .then(async () => res.json(await account.delete(req.params.id)))
        .catch(error => butil.handleQueryError(error, res));
});

router.put("/:id", (req, res) => {
    permCheck(req)
        .then(async () => res.json(await account.update(req.params.id, req.body)))
        .catch(error => butil.handleQueryError(error, res));
});

module.exports = router;
