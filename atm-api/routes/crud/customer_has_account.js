const express = require("express");
const router = express.Router();
const customer_has_account = require("../../models/crud/customer_has_account");
const butil = require("../../util");

router.get("/:key?/:id?", (req, res) => {
    if(req.params.id) {
        if(req.params.key !== "account" && req.params.key !== "customer")
            throw new butil.PromiseFail("Invalid key type of " + req.params.key, 400);

        customer_has_account.getById(req.params.key, req.params.id)
            .then(result => res.json(result))
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

    customer_has_account.delete(req.params.key, req.params.id)
        .then(result => res.json(result))
        .catch(error => butil.handleQueryError(error, res));
});

router.put("/:key/:id", (req, res) => {
    if(req.params.key !== "account" && req.params.key !== "customer")
            throw new butil.PromiseFail("Invalid key type of " + req.params.key, 400);

    customer_has_account.update(req.params.key, req.params.id, req.body)
        .then(result => res.json(result))
        .catch(error => butil.handleQueryError(error, res));
});

module.exports = router;
