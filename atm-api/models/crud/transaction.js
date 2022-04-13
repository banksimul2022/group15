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
    } 
};
