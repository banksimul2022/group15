const butil = require("./util");

module.exports = require("mysql").createPool({
    host: butil.nshcl(process.env.SQL_HOST, "localhost"),
    database: butil.nshcl(process.env.SQL_DATABASE, "bankdb"),

    user: butil.nshcl(process.env.SQL_USER, "bankuser"),
    password: butil.nshcl(process.env.SQL_PASSWORD, "bankpass"),
});
