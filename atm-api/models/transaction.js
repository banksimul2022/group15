const db = require('../database');

const transaction = {
  getById: function(id, callback) {
    return db.query('select * from transaction where transactionid=?', [id], callback);
  },
  getAll: function(callback) {
    return db.query('select * from transaction', callback);
  },
  add: function(transaction, callback) {
    console.log(transaction)
    return db.query(
      'insert into transaction (accountId,timestamp,toAccount,type,value,cardnumber) values(?,?,?,?,?,?)',
      [ transaction.accountId, transaction.timestamp, 
        transaction.toAccount, transaction.type, transaction.value, transaction.cardnumber],
      callback
    );
  },
  delete: function(id, callback) {
    return db.query('delete from transaction where transactionid=?', [id], callback);
  }, 
  update: function(id, transaction, callback) {
    return db.query(
      'update transaction set accountId=?, timestamp=?, toAccount=?, type=?, value=?, cardnumber=? where transactionid=?',
      [ transaction.accountId, transaction.timestamp,
         transaction.toAccount, transaction.type, transaction.value, transaction.cardnumber, id],
      callback
    ); 
  } 
};
module.exports = transaction;