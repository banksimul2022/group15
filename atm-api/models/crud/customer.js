const db = require("../../database");

module.exports = {
    getById: (id, callback) => {
        return db.query("SELECT * FROM customer WHERE customerId=?", [ id ], callback);
    },
    getAll: (callback) => {
        return db.query("SELECT * FROM customer", callback);
    },
    add: (customer, callback) => {
        return db.query("INSERT INTO customer (firstName, lastName, address, phone) VALUES(?, ?, ?, ?)", [ customer.firstName, customer.lastName, customer.address, customer.phone ], callback);
    },
    delete: (id, callback) => {
        return db.query("DELETE FROM customer WHERE customerId=?", [ id ], callback);
    },
    update: (id, customer, callback) => {
        return db.query("UPDATE customer SET firstName=?, lastName=?, address=?, phone=? WHERE customerId=?", [ customer.firstName, customer.lastName, customer.address, customer.phone, id ], callback);
    }
};
