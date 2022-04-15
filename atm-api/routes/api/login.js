const customer = require("../../models/crud/customer");
const card = require("../../models/crud/card");
const errors = require("../../errors");
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

    card.getByCardNumber(req.body.card_number)
        .then(dbRes => {
            if(dbRes.length < 1) {
                // Even though we return the same error message/code for an invalid customerId/pin
                // You could still probably figure this out by using a timing attack
                throw new errors.PublicAPIError("Invalid customerId or pin", errors.codes.ERR_INVALID_CREDENTIALS, 401);
            }

            return dbRes[0];
        })
        .then(async data => {
            // Should always have atleast one result as a card must be linked to a customer
            const dbRes = await customer.getById(data.customerId);
            data["permissions"] = dbRes[0].permissions;
            return data;
        })
        .then(async data => {
            const match = await bcrypt.compare(req.body.pin, data.pin);
            data["match"] = match;
            return data;
        })
        .then(data => {
            if(!data["match"]) {
                // Same as above
                throw new errors.PublicAPIError("Invalid customerId or pin", errors.codes.ERR_INVALID_CREDENTIALS, 401);
            }

            res.json({
                "token": jwt.sign({
                    card_number: req.body.card_number,
                    permissions: data.permissions,
                    customerId: data.customerId,
                    accountId: data.accountId
                }, process.env.JWT_SECRET, { expiresIn: tokenTTL + "s" }),
                "ttl": tokenTTL
            });
        })
        .catch(err => {
            if(err instanceof errors.PublicAPIError) {
                res.status(err.status);
                res.json({ error: err.code, message: err.message });
                return;
            }

            res.status(500); // Internal Server Error
            res.json({
                error: errors.codes.ERR_UNKNOWN,
                message: "An unknown error occured during the processing of the login request",
                detail: err
            });
        });
});

module.exports = router;
