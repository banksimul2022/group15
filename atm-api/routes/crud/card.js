const express = require("express");
const router = express.Router();
const card = require("../../models/crud/card");
const butil = require("../../util");

router.get("/:id?", (req, res) => {
    if(req.params.id) {
        card.getById(req.params.id).then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
    } else {
        card.getAll().then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
    }
});

router.post("/", (req, res) => {
    card.add(req.body).then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
});

router.delete("/:id", (req, res) => {
    card.delete(req.params.id).then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
});

router.put("/:id", (req, res) => {
    card.update(req.params.id, req.body).then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
});

module.exports = router;
