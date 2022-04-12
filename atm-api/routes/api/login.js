const card = require('../../models/crud/card');
const jwt = require('jsonwebtoken');
const bcrypt = require('bcryptjs');

const router = require('express').Router();

const tokenTTL = 1800; // Token time to live in seconds

router.post("/", (req, res) => {
    if(!req.body.card_number || !req.body.pin) {
        res.status(400); // Bad Request
        res.json({ "error": "The card_number or pin field was not set" });
        return;
    }

    card.getPin(req.body.card_number, (err, dbRes) => {
        if(err) {
            res.status(500); // Internal Server Error
            res.json(err);
            return;
        }

        if(dbRes.length < 1) {
            res.status(404); // Not Found
            res.json({ "error": "Unknown card_number" });
            return;
        }

        bcrypt.compare(req.body.pin, dbRes[0].pin, (err, compRes) => {
            if(!compRes) {
                res.status(403); // Forbidden
                res.json({ "error": "Invalid pin" });
                return;
            }

            res.json({
                "token": jwt.sign({
                    card_number: req.body.card_number,
                    permissions: Number.MAX_SAFE_INTEGER // Use max value for now so that everyone has full permissions to everything
                }, process.env.JWT_SECRET, { expiresIn: tokenTTL + "s" }),
                "ttl": tokenTTL
            });
        });
    });
});

module.exports = router;
