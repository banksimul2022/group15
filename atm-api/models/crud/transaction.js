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
            "INSERT INTO transaction (accountId, timestamp, toAccount, type, sum, cardNumber) VALUES(?, ?, ?, ?, ?, ?)",
            [ transaction.accountId, transaction.timestamp, transaction.toAccount, transaction.type, transaction.sum, transaction.cardNumber ]
        );
    },
    delete: (id) => {
        return db.query("DELETE FROM transaction WHERE transactionId=?", [id]);
    }, 
    update: (id, transaction) => {
        return db.query(
            "UPDATE transaction SET accountId=?, timestamp=?, toAccount=?, type=?, sum=?, cardNumber=? WHERE transactionId=?",
            [ transaction.accountId, transaction.timestamp, transaction.toAccount, transaction.type, transaction.sum, transaction.cardNumber, id ]
        ); 
    },
    fromOffset: async (accountId, offset, max) => {
        // Use 'WHERE id > offset LIMIT max' instead of 'LIMIT offset,max' to increase query performance
        // https://stackoverflow.com/a/4502392
        return await db.query(
            "SELECT * FROM transaction WHERE transactionId > ? AND accountId = ? ORDER BY transactionId LIMIT ?",
            [ offset, accountId, max ]
        );
    }
};
