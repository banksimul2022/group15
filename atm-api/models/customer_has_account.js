const db = require("../database");

module.exports = {
    getById: (id, callback) => {
        return db.query("SELECT * FROM customer_has_account WHERE customerId=?", [ id ], callback);
    },
    getAll: (callback) => {
        return db.query("SELECT * FROM customer_has_account", callback);
    },
    add: (customer_has_account, callback) => {
        
        return db.query("INSERT INTO customer_has_account (customerId, accountId) values(?, ?)", [ customer_has_account.customerId, customer_has_account.accountId], callback);
    },
    delete: (id, callback) => {
        return db.query("DELETE FROM customer_has_account WHERE customerId=?", [ id ], callback);
    },
    update: ( id, customer_has_account, callback) => {
        return db.query("UPDATE customer_has_account SET customerId=?, accountId=? WHERE customerId=?", [ customer_has_account.customerId, customer_has_account.accountId, id], callback);
    }
};