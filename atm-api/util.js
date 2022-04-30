const card = require("./models/crud/card");
const errors = require("./errors");
const jwt = require("jsonwebtoken");
const bcrypt = require('bcryptjs');
const util = require("util");

const jwtVeifyPromise = util.promisify(jwt.verify);

const throwIfDenied = (perm, token) => {
    if((token.permissions & perm) <= 0)
        throw new errors.PermissionError("You don't have permission to perform this action on the specified resource", errors.codes.ERR_NOT_ALLOWED);
};

const checkPermissions = (permFlagGetter, req, res, next) => {
    const authHeader = req.headers["authorization"];

    if(!authHeader || !authHeader.startsWith("Bearer")) {
        throw new errors.PublicAPIError("Authorization header not set or is not of the Bearer type", errors.codes.ERR_INVALID_AUTH, 401);
        return;
    }

    jwtVeifyPromise(authHeader.split(" ")[1], process.env.JWT_SECRET)
        .then(token => {
            if(permFlagGetter) {
                let perm = null;

                try {
                    perm = permFlagGetter(req);
                } catch(e) {
                    throw new errors.PrivateAPIError(
                        util.format("Failed to run permFlagGetter for '%s'! Details below.\n\n%s", req.path, e),
                        errors.codes.ERR_UNKNOWN,
                        403
                    );
                }

                if(perm === undefined || perm === null) {
                    throw new errors.PublicAPIError("Failed to determine permission flag for this action", errors.codes.ERR_UNKNOWN_PERM_FLAG, 500);
                }

                throwIfDenied(perm, token);
            }

            req.token = token;
            next();
        })
        .catch(err => {
            if(err instanceof errors.PublicAPIError) {
                res.status(err.status);
                res.json({ error: err.code, message: err.message });
                return;
            }

            if(err instanceof errors.PrivateAPIError) {
                console.error(err.message);
                res.status(err.status);
                res.json({ error: err.code, message: "Failed to validate the provided token" });
            } else {
                res.status(403);
                res.json({ error: errors.codes.ERR_UNKNOWN, message: "Failed to validate the provided token", detail: err });
            }
        });
};

module.exports = {
    // Emulates the ?? (Nullish coalescing) operator (The operator is only available in node v14 and up)
    nshcl: (value, default_value) => {
        return value === null || value === undefined ? default_value : value;
    },

    handleQueryError: (error, res) => {
        if(error instanceof errors.PublicAPIError) {
            res.status(error.status);
            res.json({ error: error.code, message: error.message });
        } else if(error.sqlMessage) {
            console.error(error);
            res.status(400);
            res.json({ error: errors.codes.ERR_DATABASE, message: error.sqlMessage });
        } else {
            console.error(error);
            res.status(500);
            res.json({ error: errors.codes.ERR_UNKNOWN, message: "An unknown error occured during the handling of the query" });
        }
    },

    permissionChecker: (permFlagGetter) => {
        return (req, res, next) => checkPermissions(permFlagGetter, req, res, next);
    },

    checkPermOnMismatch: (req, b, perm) => {
        if(req.params.id !== b) throwIfDenied(perm, req.token);
    },

    checkPermOnMismatch: (req, perm) => {
        if(req.params.id !== req.token.customerId) throwIfDenied(perm, req.token);
    },

    checkPin: async (pin, cardDat) => {
        if(cardDat.attempts < 1) {
            throw new errors.PublicAPIError("This card is locked", errors.codes.ERR_CARD_LOCKED, 403);
        }

        if(!await bcrypt.compare(pin, cardDat.pin)) {
            if(--cardDat["attempts"] < 1) {
                await card.update(cardDat["cardId"], cardDat, true);
                throw new errors.PublicAPIError("This card is locked", errors.codes.ERR_CARD_LOCKED, 403);
            }

            await card.update(cardDat["cardId"], cardDat, true);

            throw new errors.PublicAPIError("Invalid customerId or pin", errors.codes.ERR_INVALID_CREDENTIALS, 401);
        }

        if(cardDat["attempts"] !== 3) {
            cardDat["attempts"] = 3;
            await card.update(cardDat["cardId"], cardDat, true);
        }
    },

    throwIfDenied
};
