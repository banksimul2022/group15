const router = require("express").Router();

const customerRouter = require("./customer");
const accountRouter = require("./account");
const cardRouter = require("./card");
const transactionRouter = require("./transaction");
const customer_has_accountRouter = require("./customer_has_account");

router.use("/customer", customerRouter);
router.use("/account", accountRouter);
router.use("/card", cardRouter);
router.use("/transaction", transactionRouter);
router.use("/customer_has_account", customer_has_accountRouter);

module.exports = router;
