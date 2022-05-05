const db = require("../../database");

module.exports = {
    getById: (id) => {
        return db.query("SELECT * FROM account WHERE accountId=?", [ id ]);
    },
    getByNumber: (accountNumber) => {
        return db.query("SELECT * FROM account WHERE accountNumber=?", [ accountNumber ]);
    },
    getAll: () => {
        return db.query("SELECT * FROM account");
    },
    add: (account) => {
        return db.query(
            "INSERT INTO account (balance, credit, maxCredit, accountNumber) VALUES(?, ?, ?, ?)",
            [ account.balance, account.credit, account.maxCredit, account.accountNumber ]
        );
    },
    delete: (id) => {
        return db.query("DELETE FROM account WHERE accountId=?", [ id ]);
    },
    update: (id, account) => {
        return db.query(
            "UPDATE account SET balance=?, credit=?, maxCredit=?, accountNumber=? WHERE accountId=?",
            [ account.balance, account.credit, account.maxCredit, account.accountNumber, id ]
        );
    }
};
