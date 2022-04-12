const express = require("express");
const router = express.Router();
const account = require("../../models/crud/account");
const butil = require("../../util");

router.get("/:id?", (req, res) => {
    if(req.params.id) {
        account.getById(req.params.id).then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
    } else {
        account.getAll().then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
    }
});

router.post("/", (req, res) => {
    account.add(req.body).then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
});

router.delete("/:id", (req, res) => {
    account.delete(req.params.id).then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
});

router.put("/:id", (req, res) => {
    account.update(req.params.id, req.body).then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
});

module.exports = router;
