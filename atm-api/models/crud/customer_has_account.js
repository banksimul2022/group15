const db = require("../../database");

module.exports = {
    getById: (key, id) => {
        if(typeof(key) === "number") {
            return db.query(
                "SELECT * FROM customer_has_account WHERE customerId=? AND accountId=?",
                [ key, id ]
            );
        }

        return db.query(
            key === "customer" ?
                "SELECT * FROM customer_has_account WHERE customerId=?" :
                "SELECT * FROM customer_has_account WHERE accountId=?",
            [ id ]
        );
    },
    getAll: () => {
        return db.query("SELECT * FROM customer_has_account");
    },
    add: (customer_has_account) => {
        return db.query(
            "INSERT INTO customer_has_account (customerId, accountId) values(?, ?)",
            [ customer_has_account.customerId, customer_has_account.accountId ]
        );
    },
    delete: (customer, account) => {
        return db.query("DELETE FROM customer_has_account WHERE customerId=? AND accountId=?", [ customer, account ]);
    },
    update: (customer, account, customer_has_account) => {
        return db.query(
            "UPDATE customer_has_account SET customerId=?, accountId=? WHERE customerId=? AND accountId=?",
            [ customer_has_account.customerId, customer_has_account.accountId, customer, account ]
        );
    }
};
