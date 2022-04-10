const express = require("express");
const router = express.Router();
const customer = require("../../models/crud/customer");
const butil = require("../../util");

router.get("/:id?", (req, res) => {
    if(req.params.id) {
        customer.getById(req.params.id, (error, result) => butil.handleQueryResult(res, error, result));
    } else {
        customer.getAll((error, result) => butil.handleQueryResult(res, error, result));
    }
});

router.post("/", (req, res) => {
    customer.add(req.body, (error, result) => butil.handleQueryResult(res, error, result));
});

router.delete("/:id", (req, res) => {
    customer.delete(req.params.id, (error, result) => butil.handleQueryResult(res, error, result));
});

router.put("/:id", (req, res) => {
    customer.update(req.params.id, req.body, (error, result) => butil.handleQueryResult(res, error, result));
});

module.exports = router;
