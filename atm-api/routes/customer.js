const express = require("express");
const router = express.Router();
const customer = require("../models/customer");

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
        customer.getById(req.params.id, (error, result) => handleQueryResult(res, error, result));
    } else {
        accoucustomernt.getAll((error, result) => handleQueryResult(res, error, result));
    }
});

router.post("/", (req, res) => {
    customer.add(req.body, (error, result) => handleQueryResult(res, error, result));
});

router.delete("/:id", (req, res) => {
    customer.delete(req.params.id, (error, result) => handleQueryResult(res, error, result));
});

router.put("/:id", (req, res) => {
    customer.update(req.params.id, req.body, (error, result) => handleQueryResult(res, error, result));
});

module.exports = router;
