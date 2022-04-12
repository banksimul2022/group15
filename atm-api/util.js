const jwt = require("jsonwebtoken");
const util = require("util");

class PromiseFail extends Error {
    constructor(message, status) {
        super(message);
        this.status = status;
    }
}

class SilentPromiseFail extends Error {
    constructor(message) {
        super(message);
        this.name = "SILENT_PROMISE_FAIL";
    }
}

const jwtVeifyPromise = util.promisify(jwt.verify);

const checkPermissions = (permFlagGetter, req, res, next) => {
    const authHeader = req.headers["authorization"];

    if(!authHeader || !authHeader.startsWith("Bearer")) {
        res.status(401);
        res.json({ error: "Authorization header not set or is not of the Bearer type" });
        return;
    }

    jwtVeifyPromise(authHeader.split(" ")[1], process.env.JWT_SECRET)
        .then(token => {
            let perm = null;

            try {
                perm = permFlagGetter(req);
            } catch(e) {
                throw new SilentPromiseFail(util.format("Failed to run permFlagGetter for '%s'! Details below.\n\n%s", req.path, e));
            }

            if(perm === undefined || perm === null) {
                throw new PromiseFail("Failed to determine permission flag for this action", 500);
            }

            if((token.permissions & perm) <= 0) {
                throw new PromiseFail("You don't have permission to peform this action on the specified resource", 403);
            }

            req.token = token;
            next();
        })
        .catch(err => {
            if(err instanceof PromiseFail) {
                res.status(err.status);
                res.json({ error: err.message });
                return;
            }

            res.status(403);

            if(err instanceof SilentPromiseFail) {
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
        res.status(500);
        res.json(error);
    },

    permissionChecker: (permFlagGetter) => {
        return (req, res, next) => checkPermissions(permFlagGetter, req, res, next);
    },

    PromiseFail,
    SilentPromiseFail
};
