const router = require("express").Router();

const loginRouter = require("./api/login");

router.use("/login", loginRouter);

module.exports = router;
