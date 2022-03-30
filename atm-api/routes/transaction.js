const express = require("express");
const router = express.Router();
const transaction = require("../models/transaction");
const butil = require("../util");

router.get("/:id?", (req, res) => {
    if (req.params.id) {
        transaction.getById(req.params.id, (error, result) => butil.handleQueryResult(res, error, result));
    } else {
        transaction.getAll((error, result) => butil.handleQueryResult(res, error, result));
    }
});

router.post("/", (req, res) => {
    transaction.add(req.body, (error, result) => butil.handleQueryResult(res, error, result));
});

router.delete("/:id", (req, res) => {
    transaction.delete(req.params.id, (error, result) => butil.handleQueryResult(res, error, result));
});

router.put("/:id", (req, res) => {
    transaction.update(req.params.id, req.body, (error, result) => butil.handleQueryResult(res, error, result));
});

module.exports = router;
