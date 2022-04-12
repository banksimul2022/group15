const express = require("express");
const router = express.Router();
const permissions = require("../../permissions");
const card = require("../../models/crud/card");
const butil = require("../../util");

const permCheck = async (req) => {
    const result = await card.getById(req.params.id);

    if(result.length > 0) {
        butil.checkPermOnMismatch(req, result[0].customerId, permissions.crud.all.card);
    } else {
        butil.throwIfDenied(permissions.crud.all.card, req.token);
    }

    return result;
};

router.get("/:id?", (req, res) => {
    if(req.params.id) {
        permCheck(req)
            .then(result => res.json(result))
            .catch(error => butil.handleQueryError(error, res));
    } else {
        card.getAll().then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
    }
});

router.post("/", (req, res) => {
    card.add(req.body).then(result => res.json(result)).catch(error => butil.handleQueryError(error, res));
});

router.delete("/:id", (req, res) => {
    permCheck(req)
        .then(async _ => res.json(await card.delete(req.params.id)))
        .catch(error => butil.handleQueryError(error, res));
});

router.put("/:id", (req, res) => {
    permCheck(req)
        .then(async _ => res.json(await card.update(req.params.id, req.body)))
        .catch(error => butil.handleQueryError(error, res));
});

module.exports = router;
