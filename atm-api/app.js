const express = require("express");
// const path = require("path");
// const cookieParser = require("cookie-parser");
const logger = require("morgan");
const helmet = require("helmet");
const butil = require("./util");
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

app.use((err, req, res, next) => {
    if(err instanceof butil.APIError) {
        let data = { error: err.code, message: "An error occured while processing your request" };

        if(err instanceof butil.PublicAPIError) {
            data.error = err.message;
        }

        res.status(err.status);
        res.json(data);
        return;
    }

    next(err);
});

module.exports = app;
