const express = require("express");
const router = express.Router();
const customer = require("../../models/crud/customer");
const butil = require("../../util");

router.get("/:id?", (req, res) => {
    if(req.params.id) {
        customer.getById(req.params.id).then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
    } else {
        customer.getAll().then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
    }
});

router.post("/", (req, res) => {
    customer.add(req.body).then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
});

router.delete("/:id", (req, res) => {
    customer.delete(req.params.id).then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
});

router.put("/:id", (req, res) => {
    customer.update(req.params.id, req.body).then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
});

module.exports = router;
