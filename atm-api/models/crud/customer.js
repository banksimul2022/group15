const db = require("../../database");

module.exports = {
    getById: (id) => {
        return db.query("SELECT * FROM customer WHERE customerId=?", [ id ]);
    },
    getAll: () => {
        return db.query("SELECT * FROM customer");
    },
    add: (customer) => {
        return db.query("INSERT INTO customer (firstName, lastName, address, phone) VALUES(?, ?, ?, ?)", [ customer.firstName, customer.lastName, customer.address, customer.phone ]);
    },
    delete: (id) => {
        return db.query("DELETE FROM customer WHERE customerId=?", [ id ]);
    },
    update: (id, customer) => {
        return db.query("UPDATE customer SET firstName=?, lastName=?, address=?, phone=? WHERE customerId=?", [ customer.firstName, customer.lastName, customer.address, customer.phone, id ]);
    }
};
