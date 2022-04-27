const db = require("../../database");

module.exports = {
    getById: (id) => {
        return db.query("SELECT * FROM transaction WHERE transactionId=?", [ id ]);
    },
    getAll: () => {
        return db.query("SELECT * FROM transaction");
    },
    add: (transaction) => {
        return db.query(
            "INSERT INTO transaction (accountId, timestamp, accountNumber, type, sum, cardNumber) VALUES(?, ?, ?, ?, ?, ?)",
            [ transaction.accountId, transaction.timestamp, transaction.accountNumber, transaction.type, transaction.sum, transaction.cardNumber ]
        );
    },
    delete: (id) => {
        return db.query("DELETE FROM transaction WHERE transactionId=?", [id]);
    }, 
    update: (id, transaction) => {
        return db.query(
            "UPDATE transaction SET accountId=?, timestamp=?, accountNumber=?, type=?, sum=?, cardNumber=? WHERE transactionId=?",
            [ transaction.accountId, transaction.timestamp, transaction.accountNumber, transaction.type, transaction.sum, transaction.cardNumber, id ]
        ); 
    },
    ascFromOffset: (accountId, offset, max) => {
        // Use 'WHERE id > offset LIMIT max' instead of 'LIMIT offset,max' to increase query performance
        // https://stackoverflow.com/a/4502392
        return db.query(
            "SELECT * FROM transaction WHERE transactionId > ? AND accountId = ? ORDER BY transactionId LIMIT ?",
            [ offset, accountId, max ]
        );
    },
    dscFromOffset: (accountId, offset, max) => {
        // Using another trick from the link above
        return db.query(
            `SELECT transaction.* FROM (
                SELECT transactionId FROM transaction WHERE transactionId < ? AND accountId = ?
                ORDER BY transactionId DESC LIMIT ?
            ) i JOIN transaction ON transaction.transactionId = i.transactionId ORDER BY transaction.transactionId`,
            [ offset, accountId, max ]
        );
    },
    latestTransactions: (accountId, max) => {
        return db.query("SELECT * FROM transaction WHERE accountId=? ORDER BY transactionId DESC LIMIT ?", [ accountId, max ]);
    }
};
