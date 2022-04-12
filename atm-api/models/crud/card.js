const db = require("../../database");
const bcrypt = require('bcryptjs');

const saltRounds = 10;

module.exports = {
    getById: (id) => {
        return db.query("SELECT * FROM card WHERE cardId=?", [ id ]);
    },
    getAll: () => {
        return db.query("SELECT * FROM card");
    },
    add: async (card) => {
        const hash = await bcrypt.hash(card.pin, saltRounds);
        return db.query(
            "INSERT INTO card (cardId, customerId, accountId, cardNumber, locked, credit, pin) VALUES(?, ?, ?, ?, ?, ?, ?)",
            [card.cardId, card.customerId, card.accountId, card.cardNumber, card.locked, card.credit, hash]
        );
    },
    delete: (id) => {
        return db.query("DELETE FROM card WHERE cardId=?", [ id ]);
    },
    update: async (id, card) => {
        const hash = await bcrypt.hash(card.pin, saltRounds);
        return db.query(
            "UPDATE card SET customerId=?, accountId=?, cardNumber=?, locked=?, credit=?, pin=? WHERE cardId=?",
            [card.customerId, card.accountId, card.cardNumber, card.locked, card.credit, hash, id]
        );
    },
    getPinAndCustomerId: (card_number) => {
        return db.query("SELECT pin, customerId FROM card WHERE cardNumber=?", [ card_number ]);
    }
};
