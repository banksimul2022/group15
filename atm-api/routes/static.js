const account = require("../models/crud/account");
const express = require("express");
const butil = require("../util");
const path = require("path");

const router = express.Router();

const unknownProfile = (res) => {
    res.status(404);
    res.sendFile(path.join(__dirname, "../static/image/unknown_profile.png"));
};

router.use("/image/profile", butil.permissionChecker(null));

router.use("/image/profile", (req, res, next) => {
    account.getById(req.token.accountId)
        .then(dbRes => {
            const acc = dbRes[0];

            if(acc.profile === null || acc.profile === undefined) {
                unknownProfile(res);
                return;
            }

            const profile = acc.profile.indexOf("/") !== -1 ? acc.profile.substring(1) : acc.profile;
            const path = req.path.indexOf("/") !== -1 ? req.path.substring(1) : req.path;

            if(profile !== path) {
                unknownProfile(res);
                return;
            }

            next();
        })
        .catch(err => butil.handleQueryError(err, res));
});

router.use("/", express.static(path.join(__dirname, "../static")));

// Gets called when the requested path did not exist
router.use("/image/profile", (req, res, next) => {
    unknownProfile(res);
});

module.exports = router;
