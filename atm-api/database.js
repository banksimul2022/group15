// Emulates the ?? (Nullish coalescing) operator (The operator is only available in node v14 and up)
const nshcl = (value, default_value) => {
    return value === null || value === undefined ? default_value : value;
};

module.exports = require("mysql").createPool({
    host: nshcl(process.env.SQL_HOST, "localhost"),
    database: nshcl(process.env.SQL_DATABASE, "bankdb"),

    user: nshcl(process.env.SQL_USER, "bankuser"),
    password: nshcl(process.env.SQL_PASSWORD, "bankpass"),
});
