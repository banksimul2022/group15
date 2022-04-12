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
    delete: (key, id) => {
        if(typeof(key) === "number") {
            return db.query(
                "DELETE FROM customer_has_account WHERE customerId=? AND accountId=?",
                [ key, id ]
            );
        }

        return db.query(
            key === "customer" ?
                "DELETE FROM customer_has_account WHERE customerId=?" :
                "DELETE FROM customer_has_account WHERE accountId=?",
            [ id ]
        );
    },
    update: (key, id, customer_has_account) => {
        if(typeof(key) === "number") {
            return db.query(
                "UPDATE customer_has_account SET customerId=?, accountId=? WHERE customerId=? AND accountId=?",
                [ customer_has_account.customerId, customer_has_account.accountId, key, id ]
            );
        }

        return db.query(
            key === "customer" ?
                "UPDATE customer_has_account SET customerId=?, accountId=? WHERE customerId=?" :
                "UPDATE customer_has_account SET customerId=?, accountId=? WHERE accountId=?",
            [ customer_has_account.customerId, customer_has_account.accountId, id ]
        );
    }
};
