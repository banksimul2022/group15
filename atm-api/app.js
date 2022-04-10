const express = require("express");
// const path = require("path");
// const cookieParser = require("cookie-parser");
const logger = require("morgan");
const helmet = require("helmet");
const cors = require("cors");

require('dotenv').config();

const crudRouter = require("./routes/crud");
const apiRouter = require("./routes/api");

const app = express();

app.use(logger("dev"));
app.use(helmet());
app.use(cors());
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
// app.use(cookieParser()); (Not used)
// app.use(express.static(path.join(__dirname, "public")));

app.use("/crud", crudRouter);
app.use("/api", apiRouter);

module.exports = app;
