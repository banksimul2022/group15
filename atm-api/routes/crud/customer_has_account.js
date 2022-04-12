const express = require("express");
const router = express.Router();
const permissions = require("../../permissions");
const customer_has_account = require("../../models/crud/customer_has_account");
const butil = require("../../util");

const permCheck = async (req) => {
    const results = await customer_has_account.getById(req.params.key, req.params.id);

    if(req.params.key === "account") {
        if(!results.length || results.length < 1) {
            butil.throwIfDenied(permissions.crud.all.customer_has_account, req.token);
        } else {
            for(let result of results) {
                butil.checkPermOnMismatch(req, result.customerId, permissions.crud.all.customer_has_account);
            }
        }
    }

    return results;
};

router.get("/:key?/:id?", (req, res) => {
    if(req.params.id) {
        if(req.params.key !== "account" && req.params.key !== "customer")
            throw new butil.PromiseFail("Invalid key type of " + req.params.key, 400);

        if(req.params.key === "customer")
            butil.checkPermOnMismatch(req, permissions.crud.all.customer_has_account);

        permCheck(req)
            .then(results => res.json(results))
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

router.delete("/:key/:id", (req, res) => {
    if(req.params.key !== "account" && req.params.key !== "customer")
            throw new butil.PromiseFail("Invalid key type of " + req.params.key, 400);

    if(req.params.key === "customer")
        butil.checkPermOnMismatch(req, permissions.crud.all.customer_has_account);

    permCheck(req)
        .then(async _ => res.json(await customer_has_account.delete(req.params.key, req.params.id)))
        .catch(error => butil.handleQueryError(error, res));
});

router.put("/:key/:id", (req, res) => {
    if(req.params.key !== "account" && req.params.key !== "customer")
            throw new butil.PromiseFail("Invalid key type of " + req.params.key, 400);

    if(req.params.key === "customer")
        butil.checkPermOnMismatch(req, permissions.crud.all.customer_has_account);

    permCheck(req)
        .then(async _ => res.json(await customer_has_account.update(req.params.key, req.params.id, req.body)))
        .catch(error => butil.handleQueryError(error, res));
});

module.exports = router;
