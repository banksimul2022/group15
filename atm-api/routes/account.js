const express = require("express");
const router = express.Router();
const account = require("../models/account");
const butil = require("../util");

router.get("/:id?", (req, res) => {
    if(req.params.id) {
        account.getById(req.params.id, (error, result) => butil.handleQueryResult(res, error, result));
    } else {
        account.getAll((error, result) => butil.handleQueryResult(res, error, result));
    }
});

router.post("/", (req, res) => {
    account.add(req.body, (error, result) => butil.handleQueryResult(res, error, result));
});

router.delete("/:id", (req, res) => {
    account.delete(req.params.id, (error, result) => butil.handleQueryResult(res, error, result));
});

router.put("/:id", (req, res) => {
    account.update(req.params.id, req.body, (error, result) => butil.handleQueryResult(res, error, result));
});

module.exports = router;
