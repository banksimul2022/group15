const card = require('../../models/crud/card');
const jwt = require('jsonwebtoken');
const bcrypt = require('bcryptjs');

const router = require('express').Router();

const tokenTTL = 1800; // Token time to live in seconds

router.post("/", (req, res) => {
    if(!req.body.card_number || !req.body.pin) {
        res.json({ "error": "The card_number or pin field was not set" });
        res.status(400); // Bad Request
        return;
    }

    card.getPin(req.body.card_number, (err, dbRes) => {
        if(err) {
            res.json(err);
            res.status(500); // Internal Server Error
            return;
        }

        if(dbRes.length < 1) {
            res.json({ "error": "Unknown card_number" });
            res.status(404); // Not Found
            return;
        }

        bcrypt.compare(req.body.pin, dbRes[0].pin, (err, compRes) => {
            if(!compRes) {
                res.json({ "error": "Invalid pin" });
                res.status(403); // Forbidden
                return;
            }

            res.json({
                "token": jwt.sign({ card_number: req.body.card_number }, process.env.JWT_SECRET, { expiresIn: "1800s" }),
                "ttl": tokenTTL
            });
        });
    });
});

module.exports = router;
