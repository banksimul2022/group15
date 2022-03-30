const db = require("../database");

module.exports = {
    getById: (id, callback) => {
        return db.query("SELECT * FROM card WHERE cardId=?", [ id ], callback);
    },
    getAll: (callback) => {
        return db.query("SELECT * FROM card", callback);
    },
    add: (card, callback) => {
        return db.query("INSERT INTO card (cardId, customerId, accountId, cardNumber, locked, pin) VALUES(?, ?, ?, ?, ?, ?)", [ card.cardId, card.customerId, card.accountId, card.cardNumber, card.locked, card.pin ], callback);
    },
    delete: (id, callback) => {
        return db.query("DELETE FROM card WHERE cardId=?", [ id ], callback);
    },
    update: (id, card, callback) => {
        return db.query("UPDATE card SET customerId=?, accountId=?, cardNumber=?, locked=?, pin=? WHERE cardId=?", [ card.customerId, card.accountId, card.cardNumber, card.locked, card.pin, id ], callback);
    }
};
