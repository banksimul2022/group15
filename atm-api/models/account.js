const db = require("../database");

module.exports = {
    getById: (id, callback) => {
        return db.query("SELECT * FROM account WHERE accountId=?", [ id ], callback);
    },
    getAll: (callback) => {
        return db.query("SELECT * FROM account", callback);
    },
    add: (account, callback) => {
        return db.query("INSERT INTO account (credit, accountNumber) VALUES(?, ?)", [ account.credit, account.accountNumber ], callback);
    },
    delete: (id, callback) => {
        return db.query("DELETE FROM account WHERE accountId=?", [ id ], callback);
    },
    update: (id, account, callback) => {
        return db.query("UPDATE account SET credit=?, accountNumber=? WHERE accountId=?", [ account.credit, account.accountNumber, id ], callback);
    }
};