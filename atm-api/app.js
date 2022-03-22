const express = require("express");
const path = require("path");
// const cookieParser = require("cookie-parser");
const logger = require("morgan");
const helmet = require("helmet");
const cors = require("cors");

const indexRouter = require("./routes/index");
const usersRouter = require("./routes/users");

const app = express();

app.use(logger("dev"));
app.use(helmet());
app.use(cors());
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
// app.use(cookieParser()); (Not used)
// app.use(express.static(path.join(__dirname, "public")));

app.use("/", indexRouter);
app.use("/users", usersRouter);

module.exports = app;
