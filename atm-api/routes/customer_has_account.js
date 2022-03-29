const express = require("express");
const router = express.Router();
const customer_has_account = require("../models/customer_has_account");

const handleQueryResult = (res, error, result) => {
    if(error) {
        res.json(error);
        res.status(500);
        return;
    }

    res.json(result);
};

router.get("/:id?", (req, res) => {
    if(req.params.id) {
        customer_has_account.getById(req.params.id, (error, result) => handleQueryResult(res, error, result));
    } else {
        customer_has_account.getAll((error, result) => handleQueryResult(res, error, result));
    }
});

router.post("/", (req, res) => {
    customer_has_account.add(req.body, (error, result) => handleQueryResult(res, error, result));
});

router.delete("/:id", (req, res) => {
    customer_has_account.delete(req.params.id, (error, result) => handleQueryResult(res, error, result));
});

router.put("/:id", (req, res) => {
    customer_has_account.update(req.params.id, req.body, (error, result) => handleQueryResult(res, error, result));
});

module.exports = router;