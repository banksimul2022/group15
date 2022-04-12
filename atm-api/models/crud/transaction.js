const db = require("../../database");

module.exports = {
    getById: (id) => {
        return db.query("SELECT * FROM transaction WHERE transactionid=?", [ id ]);
    },
    getAll: () => {
        return db.query("SELECT * FROM transaction");
    },
    add: (transaction) => {
        return db.query(
            "INSERT INTO transaction (accountId, timestamp, toAccount, type, value, cardNumber) VALUES(?, ?, ?, ?, ?, ?)",
            [ transaction.accountId, transaction.timestamp, transaction.toAccount, transaction.type, transaction.value, transaction.cardNumber ]
        );
    },
    delete: (id) => {
        return db.query("DELETE FROM transaction WHERE transactionid=?", [id]);
    }, 
    update: (id, transaction) => {
        return db.query(
            "UPDATE transaction SET accountId=?, timestamp=?, toAccount=?, type=?, sum=?, cardNumber=? WHERE transactionid=?",
            [ transaction.accountId, transaction.timestamp, transaction.toAccount, transaction.type, transaction.sum, transaction.cardNumber, id ]
        ); 
    } 
};
