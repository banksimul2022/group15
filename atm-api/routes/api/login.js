const customer = require("../../models/crud/customer");
const card = require("../../models/crud/card");
const butil = require("../../util");
const jwt = require("jsonwebtoken");
const bcrypt = require("bcryptjs");

const router = require("express").Router();

const tokenTTL = 1800; // Token time to live in seconds

router.post("/", (req, res) => {
    if(!req.body.card_number || !req.body.pin) {
        res.status(400); // Bad Request
        res.json({ "error": "The card_number or pin field was not set" });
        return;
    }

    card.getPinAndCustomerId(req.body.card_number)
        .then(dbRes => {
            if(dbRes.length < 1) {
                throw new butil.PromiseFail("Unknown card_number", 404);
            }

            return dbRes[0];
        })
        .then(data => {
            return customer.getById(data.customerId)
                .then(dbRes => { // Should always have atleast one result as a card must be linked to a customer
                    data["permissions"] = dbRes[0].permissions;
                    return data;
                });
        })
        .then(data => {
            return bcrypt.compare(req.body.pin, data.pin)
                .then(match => {
                    data["match"] = match;
                    return data;
                });
        })
        .then(data => {
            if(!data["match"]) {
                throw new butil.PromiseFail("Invalid pin", 403);
            }

            res.json({
                "token": jwt.sign({
                    card_number: req.body.card_number,
                    permissions: data.permissions
                }, process.env.JWT_SECRET, { expiresIn: tokenTTL + "s" }),
                "ttl": tokenTTL
            });
        })
        .catch(err => {
            if(err instanceof butil.PromiseFail) {
                res.status(err.status);
                res.json({ error: err.message });
                return;
            }

            res.status(500); // Internal Server Error
            res.json(err);
        });
});

module.exports = router;
