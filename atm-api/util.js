const jwt = require("jsonwebtoken");
const util = require("util");

// Used when the error should not be sent to the requester
class PrivateAPIError extends Error {
    constructor(message) {
        super(message);
    }
}

// Used when the error can be sent to the requester
class PublicAPIError extends Error {
    constructor(message, status) {
        super(message);
        this.status = status;
    }
}

class PermissionError extends PublicAPIError {
    constructor(message) {
        super(message, 403);
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
        res.status(401);
        res.json({ error: "Authorization header not set or is not of the Bearer type" });
        return;
    }

    jwtVeifyPromise(authHeader.split(" ")[1], process.env.JWT_SECRET)
        .then(token => {
            if(permFlagGetter) {
                let perm = null;

                try {
                    perm = permFlagGetter(req);
                } catch(e) {
                    throw new PrivateAPIError(util.format("Failed to run permFlagGetter for '%s'! Details below.\n\n%s", req.path, e));
                }

                if(perm === undefined || perm === null) {
                    throw new PublicAPIError("Failed to determine permission flag for this action", 500);
                }

                throwIfDenied(perm, token);
            }

            req.token = token;
            next();
        })
        .catch(err => {
            if(err instanceof PublicAPIError) {
                res.status(err.status);
                res.json({ error: err.message });
                return;
            }

            res.status(403);

            if(err instanceof PrivateAPIError) {
                console.error(err.message);
                res.json({ error: "Failed to validate the provided token" });
            } else {
                res.json({ error: "Failed to validate the provided token", detail: err });
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
            res.json({ error: error.message });
        } else if(error.sqlMessage) {
            console.error(error);
            res.status(400);
            res.json({ error: error.sqlMessage });
        } else {
            console.error(error);
            res.status(500);
            res.json({ error: "An unknown error occured during the handling of the query" });
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
