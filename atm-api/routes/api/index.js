const butil = require("../../util");

const router = require("express").Router();

const dataRouter = require("./data");
const loginRouter = require("./login");

router.use("/login", loginRouter);

router.use(butil.permissionChecker(null));

router.use("/", dataRouter);


module.exports = router;
