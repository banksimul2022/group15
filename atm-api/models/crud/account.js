const db = require("../../database");

module.exports = {
    getById: (id) => {
        return db.query("SELECT * FROM account WHERE accountId=?", [ id ]);
    },
    getAll: () => {
        return db.query("SELECT * FROM account");
    },
    add: (account) => {
        return db.query("INSERT INTO account (balance, accountNumber) VALUES(?, ?)", [ account.balance, account.accountNumber ]);
    },
    delete: (id) => {
        return db.query("DELETE FROM account WHERE accountId=?", [ id ]);
    },
    update: (id, account) => {
        return db.query("UPDATE account SET balance=?, accountNumber=? WHERE accountId=?", [ account.balance, account.accountNumber, id ]);
    }
};
