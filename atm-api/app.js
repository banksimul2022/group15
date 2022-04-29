const express = require("express");
const path = require("path");
const logger = require("morgan");
const helmet = require("helmet");
const errors = require("./errors");
const cors = require("cors");

require('dotenv').config();

const crudRouter = require("./routes/crud");
const apiRouter = require("./routes/api");
const staticRouter = require("./routes/static");

const app = express();

app.use(logger("dev"));
app.use(helmet());
app.use(cors());
app.use(express.json());
app.use(express.urlencoded({ extended: false }));

app.use("/crud", crudRouter);
app.use("/api", apiRouter);
app.use("/static", staticRouter);

app.use((err, req, res, next) => {
    if(err instanceof errors.PublicAPIError) {
        let data = { error: err.code, message: "An error occured while processing your request" };

        if(err instanceof errors.PublicAPIError) {
            data.message = err.message;
        }

        res.status(err.status);
        res.json(data);
        return;
    }

    next(err);
});

module.exports = app;
