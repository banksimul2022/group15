const express = require("express");
const router = express.Router();
const card = require("../models/card");
const butil = require("../util");

router.get("/:id?", (req, res) => {
    if(req.params.id) {
        card.getById(req.params.id, (error, result) => butil.handleQueryResult(res, error, result));
    } else {
        card.getAll((error, result) => butil.handleQueryResult(res, error, result));
    }
});

router.post("/", (req, res) => {
    card.add(req.body, (error, result) => butil.handleQueryResult(res, error, result));
});

router.delete("/:id", (req, res) => {
    card.delete(req.params.id, (error, result) => butil.handleQueryResult(res, error, result));
});

router.put("/:id", (req, res) => {
    card.update(req.params.id, req.body, (error, result) => butil.handleQueryResult(res, error, result));
});

module.exports = router;
