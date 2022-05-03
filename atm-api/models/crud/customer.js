const db = require("../../database");

module.exports = {
    getById: (id) => {
        return db.query("SELECT * FROM customer WHERE customerId=?", [ id ]);
    },
    getAll: () => {
        return db.query("SELECT * FROM customer");
    },
    add: (customer) => {
        return db.query(
            "INSERT INTO customer (permissions, firstName, lastName, address, phone, profile) VALUES(?, ?, ?, ?, ?, ?)",
            [ customer.permissions, customer.firstName, customer.lastName, customer.address, customer.phone, customer.profile ]
        );
    },
    delete: (id) => {
        return db.query("DELETE FROM customer WHERE customerId=?", [ id ]);
    },
    update: (id, customer) => {
        return db.query(
            "UPDATE customer SET permissions=?, firstName=?, lastName=?, address=?, phone=?, profile=? WHERE customerId=?",
            [ customer.permissions, customer.firstName, customer.lastName, customer.address, customer.phone, customer.profile, id ]
        );
    }
};
