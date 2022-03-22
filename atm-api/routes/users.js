var express = require("express");
var router = express.Router();

/* GET users listing. */
router.get("/", function(req, res, next) {
  res.send("Welcome! This is the /users endpoint");
});

module.exports = router;
