const express = require("express");
const router = express.Router();
const transaction = require("../../models/crud/transaction");
const butil = require("../../util");

router.get("/:id?", (req, res) => {
    if (req.params.id) {
        transaction.getById(req.params.id).then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
    } else {
        transaction.getAll().then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
    }
});

router.post("/", (req, res) => {
    transaction.add(req.body).then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
});

router.delete("/:id", (req, res) => {
    transaction.delete(req.params.id).then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
});

router.put("/:id", (req, res) => {
    transaction.update(req.params.id, req.body).then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
});

module.exports = router;
