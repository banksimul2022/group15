const db = require("../../database");

module.exports = {
    getById: (id) => {
        return db.query("SELECT * FROM customer_has_account WHERE customerId=?", [ id ]);
    },
    getAll: () => {
        return db.query("SELECT * FROM customer_has_account");
    },
    add: (customer_has_account) => {
        
        return db.query("INSERT INTO customer_has_account (customerId, accountId) values(?, ?)", [ customer_has_account.customerId, customer_has_account.accountId]);
    },
    delete: (id) => {
        return db.query("DELETE FROM customer_has_account WHERE customerId=?", [ id ]);
    },
    update: ( id, customer_has_account) => {
        return db.query("UPDATE customer_has_account SET customerId=?, accountId=? WHERE customerId=?", [ customer_has_account.customerId, customer_has_account.accountId, id]);
    }
};
