const jwt = require("jsonwebtoken");
const util = require("util");

const checkPermissions = (permFlagGetter, req, res, next) => {
    const authHeader = req.headers["authorization"];

    if(!authHeader || !authHeader.startsWith("Bearer")) {
        res.status(401);
        res.json({ error: "Authorization header not set or is not of the Bearer type" });
        return;
    }

    jwt.verify(authHeader.split(" ")[1], process.env.JWT_SECRET, (err, token) => {
        if(err) {
            res.status(403);
            res.json({ error: "Failed to validate the provided token", detail: err });
            return;
        }

        let perm = null;
        try {
            perm = permFlagGetter(req);
        } catch(e) {
            console.error(util.format("Failed to run permFlagGetter for '%s'! Details below.\n\n%s", req.path, e));
        }

        if(perm === undefined || perm === null) {
            res.status(500);
            res.json({ error: "Failed to determine permission flag for this action" });
            return;
        }

        if((token.permissions & perm) <= 0) {
            res.status(403);
            res.json({ error: "You don't have permission to peform this action on the specified resource" });
            return;
        }

        req.token = token;
        next();
    });
};

module.exports = {
    // Emulates the ?? (Nullish coalescing) operator (The operator is only available in node v14 and up)
    nshcl: (value, default_value) => {
        return value === null || value === undefined ? default_value : value;
    },

    handleQueryResult: (res, error, result) => {
        if(error) {
            res.status(500);
            res.json(error);
            return;
        }

        res.json(result);
    },

    permissionChecker: (permFlagGetter) => {
        return (req, res, next) => checkPermissions(permFlagGetter, req, res, next);
    }
};
