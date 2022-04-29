const express = require("express");
const path = require("path");

const router = express.Router();

router.use("/", express.static(path.join(__dirname, "../static")));

// Gets called when the requested path did not exist
router.use("/image/profile", (req, res, next) => {
    res.status(404);
    res.sendFile(path.join(__dirname, "../static/image/unknown_profile.png"));
});

module.exports = router;
