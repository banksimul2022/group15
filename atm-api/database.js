const butil = require("./util");
const util = require("util");

const dbConnection = require("mysql").createPool({
    host: butil.nshcl(process.env.SQL_HOST, "localhost"),
    database: butil.nshcl(process.env.SQL_DATABASE, "bankdb"),

    user: butil.nshcl(process.env.SQL_USER, "bankuser"),
    password: butil.nshcl(process.env.SQL_PASSWORD, "bankpass"),
});

const originalQueryFunction = dbConnection.query;

dbConnection.query = util.promisify(originalQueryFunction);

module.exports = dbConnection;