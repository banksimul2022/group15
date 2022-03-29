const express = require("express");
// const path = require("path");
// const cookieParser = require("cookie-parser");
const logger = require("morgan");
const helmet = require("helmet");
const cors = require("cors");

const customerRouter = require("./routes/customer");
const accountRouter = require("./routes/account");
const transactionRouter = require("./routes/transaction");

const app = express();

app.use(helmet());
app.use(cors());
app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
//app.use(cookieParser()); (Not used)
//app.use(express.static(path.join(__dirname, 'public')));

app.use("/customer", customerRouter);
app.use("/account", accountRouter);
app.use("/transaction", transactionRouter);

module.exports = app;
