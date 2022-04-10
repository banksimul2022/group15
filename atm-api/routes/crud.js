const router = require("express").Router();

const customerRouter = require("./crud/customer");
const accountRouter = require("./crud/account");
const cardRouter = require("./crud/card");
const transactionRouter = require("./crud/transaction");
const customer_has_accountRouter = require("./crud/customer_has_account");

router.use("/customer", customerRouter);
router.use("/account", accountRouter);
router.use("/card", cardRouter);
router.use("/transaction", transactionRouter);
router.use("/customer_has_account", customer_has_accountRouter);

module.exports = router;
