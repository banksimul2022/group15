const express = require("express");
const router = express.Router();
const permissions = require("../../permissions");
const customer_has_account = require("../../models/crud/customer_has_account");
const errors = require("../../errors");
const butil = require("../../util");

router.get("/:key?/:id?", (req, res) => {
    if(req.params.id) {
        if(req.params.key !== "account" && req.params.key !== "customer")
            throw new butil.PublicAPIError("Invalid key type of " + req.params.key, errors.codes.ERR_INVALID_KEY, 400);

        if(req.params.key === "customer")
            butil.checkPermOnMismatch(req, permissions.crud.all.customer_has_account);

        customer_has_account.getById(req.params.key, req.params.id)
            .then(results => {
                if(req.params.key === "account") {
                    if(!results.length || results.length < 1) {
                        butil.throwIfDenied(permissions.crud.all.customer_has_account, req.token);
                    } else {
                        for(let result of results) {
                            butil.checkPermOnMismatch(req, result.customerId, permissions.crud.all.customer_has_account);
                        }
                    }
                }

                res.json(results);
            })
            .catch(error => butil.handleQueryError(error, res));
    } else {
        customer_has_account.getAll()
            .then(result => res.json(result))
            .catch(error => butil.handleQueryError(error, res));
    }
});

router.post("/", (req, res) => {
    customer_has_account.add(req.body)
        .then(result => res.json(result))
        .catch(error => butil.handleQueryError(error, res));
});

router.delete("/:customer/:account", (req, res) => {
    if(req.params.customer !== req.token.customerId)
        butil.throwIfDenied(permissions.crud.all.customer_has_account, req.token);

    customer_has_account.delete(req.params.customer, req.params.account)
        .then(result => res.json(result))
        .catch(error => butil.handleQueryError(error, res));
});

router.put("/:customer/:account", (req, res) => {
    if(req.params.customer !== req.token.customerId)
        butil.throwIfDenied(permissions.crud.all.customer_has_account, req.token);

    customer_has_account.update(req.params.customer, req.params.account, req.body)
        .then(result => res.json(result))
        .catch(error => butil.handleQueryError(error, res));
});

module.exports = router;
