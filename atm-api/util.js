const errors = require("./errors");
const jwt = require("jsonwebtoken");
const util = require("util");

class APIError extends Error {
    constructor(message, code, status) {
        super(message);
        this.code = code;
        this.status = status;
    }
}

// Used when the error details should not be sent to the requester
class PrivateAPIError extends APIError {
    constructor(message, code, status) {
        super(message, code, status);
    }
}

// Used when the error details can be sent to the requester
class PublicAPIError extends APIError {
    constructor(message, code, status) {
        super(message, code, status);
    }
}

class PermissionError extends PublicAPIError {
    constructor(message, code) {
        super(message, code, 403);
    }
}

const jwtVeifyPromise = util.promisify(jwt.verify);

const throwIfDenied = (perm, token) => {
    if((token.permissions & perm) <= 0)
        throw new PermissionError("You don't have permission to peform this action on the specified resource", 403);
};

const checkPermissions = (permFlagGetter, req, res, next) => {
    const authHeader = req.headers["authorization"];

    if(!authHeader || !authHeader.startsWith("Bearer")) {
        throw new PublicAPIError("Authorization header not set or is not of the Bearer type", errors.codes.ERR_INVALID_AUTH, 401);
        return;
    }

    jwtVeifyPromise(authHeader.split(" ")[1], process.env.JWT_SECRET)
        .then(token => {
            if(permFlagGetter) {
                let perm = null;

                try {
                    perm = permFlagGetter(req);
                } catch(e) {
                    throw new PrivateAPIError(
                        util.format("Failed to run permFlagGetter for '%s'! Details below.\n\n%s", req.path, e),
                        errors.codes.ERR_UNKNOWN,
                        403
                    );
                }

                if(perm === undefined || perm === null) {
                    throw new PublicAPIError("Failed to determine permission flag for this action", errors.codes.ERR_UNKNOWN_PERM_FLAG, 500);
                }

                throwIfDenied(perm, token);
            }

            req.token = token;
            next();
        })
        .catch(err => {
            if(err instanceof PublicAPIError) {
                res.status(err.status);
                res.json({ error: err.code, message: err.message });
                return;
            }

            if(err instanceof PrivateAPIError) {
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
        if(error instanceof PublicAPIError) {
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

    throwIfDenied,

    PublicAPIError,
    PrivateAPIError
};
