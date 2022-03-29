const express = require("express");
const router = express.Router();
const account = require("../models/account");

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
        account.getById(req.params.id, (error, result) => handleQueryResult(res, error, result));
    } else {
        account.getAll((error, result) => handleQueryResult(res, error, result));
    }
});

router.post("/", (req, res) => {
    account.add(req.body, (error, result) => handleQueryResult(res, error, result));
});

router.delete("/:id", (req, res) => {
    account.delete(req.params.id, (error, result) => handleQueryResult(res, error, result));
});

router.put("/:id", (req, res) => {
    account.update(req.params.id, req.body, (error, result) => handleQueryResult(res, error, result));
});

module.exports = router;
