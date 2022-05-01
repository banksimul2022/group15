const util = require("util");

// :( The util.js file provides this function but this causes an recursive require with card.js as utiljs needs card.js 
const nshcl = (value, default_value) => {
    return value === null || value === undefined ? default_value : value;
};

const dbConnection = require("mysql").createPool({
    host: nshcl(process.env.SQL_HOST, "localhost"),
    database: nshcl(process.env.SQL_DATABASE, "bankdb"),

    user: nshcl(process.env.SQL_USER, "bankuser"),
    password: nshcl(process.env.SQL_PASSWORD, "bankpass"),
});

const originalQueryFunction = dbConnection.query;

dbConnection.query = util.promisify(originalQueryFunction);

module.exports = dbConnection;
