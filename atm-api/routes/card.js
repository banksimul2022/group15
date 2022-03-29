const express = require("express");
const router = express.Router();
const card = require("../models/card");

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
        card.getById(req.params.id, (error, result) => handleQueryResult(res, error, result));
    } else {
        card.getAll((error, result) => handleQueryResult(res, error, result));
    }
});

router.post("/", (req, res) => {
    card.add(req.body, (error, result) => handleQueryResult(res, error, result));
});

router.delete("/:id", (req, res) => {
    card.delete(req.params.id, (error, result) => handleQueryResult(res, error, result));
});

router.put("/:id", (req, res) => {
    card.update(req.params.id, req.body, (error, result) => handleQueryResult(res, error, result));
});

module.exports = router;
