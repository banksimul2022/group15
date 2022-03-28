const express = require("express");
// const path = require("path");
// const cookieParser = require("cookie-parser");
const logger = require("morgan");
const helmet = require("helmet");
const cors = require("cors");

const customerRouter = require("./routes/customer");

const app = express();

app.use(logger("dev"));
app.use(helmet());
app.use(cors());
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
// app.use(cookieParser()); (Not used)
// app.use(express.static(path.join(__dirname, "public")));

app.use("/customer", customerRouter);

module.exports = app;
