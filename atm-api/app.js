const express = require("express");
// const path = require("path");
// const cookieParser = require("cookie-parser");
const logger = require("morgan");
const helmet = require("helmet");
const cors = require("cors");

const customerRouter = require("./routes/customer");
const accountRouter = require("./routes/account");
const cardRouter = require("./routes/card");
const transactionRouter = require("./routes/transaction");
const customer_has_accountRouter = require("./routes/customer_has_account");


const app = express();

app.use(logger('dev'));
app.use(helmet());
app.use(cors());
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
// app.use(cookieParser()); (Not used)
// app.use(express.static(path.join(__dirname, 'public')));


app.use("/customer", customerRouter);
app.use("/account", accountRouter);
app.use("/card", cardRouter);
app.use("/transaction", transactionRouter);
app.use("/customer_has_account", customer_has_accountRouter);

module.exports = app;
