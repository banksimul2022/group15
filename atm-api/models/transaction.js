const db = require("../database");

module.exports = {
    getById: (id, callback) => {
        return db.query("SELECT * FROM transaction WHERE transactionid=?", [ id ], callback);
    },
    getAll: (callback) => {
        return db.query("SELECT * FROM transaction", callback);
    },
    add: (transaction, callback) => {
        return db.query(
            "INSERT INTO transaction (accountId, timestamp, toAccount, type, value, cardNumber) VALUES(?, ?, ?, ?, ?, ?)",
            [ transaction.accountId, transaction.timestamp, transaction.toAccount, transaction.type, transaction.value, transaction.cardNumber ],
            callback
        );
    },
    delete: (id, callback) => {
        return db.query("DELETE FROM transaction WHERE transactionid=?", [id], callback);
    }, 
    update: (id, transaction, callback) => {
        return db.query(
            "UPDATE transaction SET accountId=?, timestamp=?, toAccount=?, type=?, sum=?, cardNumber=? WHERE transactionid=?",
            [ transaction.accountId, transaction.timestamp, transaction.toAccount, transaction.type, transaction.sum, transaction.cardNumber, id ],
            callback
        ); 
    } 
};
