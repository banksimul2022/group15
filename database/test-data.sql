USE bankdb;

TRUNCATE TABLE customer_has_account;
TRUNCATE TABLE transaction;
TRUNCATE TABLE card;
DELETE FROM customer;
DELETE FROM account;

-- customer
-- +------------+--------------+------+-----+---------+----------------+
-- | Field      | Type         | Null | Key | Default | Extra          |
-- +------------+--------------+------+-----+---------+----------------+
-- | customerId | int          | NO   | PRI | NULL    | auto_increment |
-- | firstName  | int unsigned | NO   |     | NULL    |                |
-- | firstName  | varchar(45)  | NO   |     | NULL    |                |
-- | lastName   | varchar(45)  | NO   |     | NULL    |                |
-- | address    | varchar(45)  | NO   |     | NULL    |                |
-- | phone      | varchar(45)  | YES  |     | NULL    |                |
-- | profile    | varchar(20)  | YES  |     | NULL    |                |
-- +------------+--------------+------+-----+---------+----------------+
INSERT INTO customer VALUES(1, 4294967295, 'Roope', 'Ankka', 'Rahasäiliö 1',    10000, NULL);
INSERT INTO customer VALUES(2, 0,          'Aku',   'Ankka', 'Paratiisitie 13', 43738, '2.png');
INSERT INTO customer VALUES(3, 1,          'Iines', 'Ankka', 'Lepolahti 8',     12464, NULL);
INSERT INTO customer VALUES(4, 0,          'Hupu',  'Ankka', 'Paratiisitie 13', 43738, NULL);
INSERT INTO customer VALUES(5, 0,          'Tupu',  'Ankka', 'Paratiisitie 13', 43738, NULL);
INSERT INTO customer VALUES(6, 0,          'Lupu',  'Ankka', 'Paratiisitie 13', 43738, NULL);


-- account
-- +---------------+---------------+------+-----+---------+----------------+
-- | Field         | Type          | Null | Key | Default | Extra          |
-- +---------------+---------------+------+-----+---------+----------------+
-- | accountId     | int           | NO   | PRI | NULL    | auto_increment |
-- | balance       | decimal(14,2) | NO   |     | NULL    |                |
-- | credit        | decimal(14,2) | NO   |     | NULL    |                |
-- | maxCredit     | decimal(14,2) | NO   |     | 3000.00 |                |
-- | accountNumber | varchar(45)   | NO   |     | NULL    |                |
-- +---------------+---------------+------+-----+---------+----------------+
INSERT INTO account VALUES(1, 845.89,    60.0,   DEFAULT, 111111);
INSERT INTO account VALUES(2, 788852.00, 0,      DEFAULT, 222222);
INSERT INTO account VALUES(3, -18.78,    180.08, 0,       333333);
INSERT INTO account VALUES(4, 313.00,    18.24,  50,      444444);
INSERT INTO account VALUES(5, 878451.87, 0,      DEFAULT, 555555);


-- card
-- +------------+--------------+------+-----+---------+----------------+
-- | Field      | Type         | Null | Key | Default | Extra          |
-- +------------+--------------+------+-----+---------+----------------+
-- | cardId     | int          | NO   | PRI | NULL    | auto_increment |
-- | customerId | int          | NO   | MUL | NULL    |                |
-- | accountId  | int          | NO   | MUL | NULL    |                |
-- | cardNumber | varchar(10)  | NO   |     | NULL    |                |
-- | attempts   | tinyint      | NO   |     | 3       |                |
-- | credit     | tinyint      | NO   |     | NULL    |                |
-- | pin        | varchar(255) | NO   |     | NULL    |                |
-- +------------+--------------+------+-----+---------+----------------+
INSERT INTO card VALUES(1, 1, 1, '11111111', DEFAULT, 0, '$2a$10$HcdMFpD7dc1iMXjraAO6LucN/duerFdGpOSSJZ/E02gGs5Ty06WqK'); -- 9599
INSERT INTO card VALUES(2, 2, 2, '22222222', DEFAULT, 0, '$2a$10$/8brIP8WQemls9sdo2QvHeLcqHmRhk494mmEL/ED1fgZ4rKRhnGIu'); -- 7862
INSERT INTO card VALUES(3, 3, 3, '33333333', DEFAULT, 0, '$2a$10$mc1ePdHkcL.zfG/dTJm9OuKPZSg0tAYJnfnDli728agkzIBCAFlbG'); -- 5671
INSERT INTO card VALUES(4, 4, 4, '44444444', DEFAULT, 0, '$2a$10$pBknN6IdN5dFbTqVvsKx7.3BHtM7lbig407AX0I1U9sazPGvMfEXq'); -- 7225
INSERT INTO card VALUES(5, 5, 5, '55555555', DEFAULT, 0, '$2a$10$tIx1gnLDCE22Me3GXSKSmOl.4xwTTCjxfNqoc3rYHpxDiAdyzo/hS'); -- 2012
INSERT INTO card VALUES(6, 1, 2, '10101010', DEFAULT, 0, '$2a$10$PJRniMuycRkjBJ6svRtWj.eJE9gfeHXg75k1WBgjHYlLmkNAGEdry'); -- 4078


-- transaction
-- +---------------+--------------------------------------------------------------------------------+------+-----+---------+----------------+
-- | Field         | Type                                                                           | Null | Key | Default | Extra          |
-- +---------------+--------------------------------------------------------------------------------+------+-----+---------+----------------+
-- | transactionId | int                                                                            | NO   | PRI | NULL    | auto_increment |
-- | accountId     | int                                                                            | NO   | PRI | NULL    |                |
-- | timestamp     | datetime                                                                       | NO   |     | NULL    |                |
-- | type          | enum('DEPOSIT', 'WITHDRAW', 'CREDIT_WITHDRAW', 'TRANSFER_TO', 'TRANSFER_FROM') | NO   |     | NULL    |                |
-- | sum           | decimal(14,2)                                                                  | NO   |     | NULL    |                |
-- | cardNumber    | varchar(10)                                                                    | YES  |     | NULL    |                |
-- | accountNumber | varchar(45)                                                                    | YES  |     | NULL    |                |
-- +---------------+--------------------------------------------------------------------------------+------+-----+---------+----------------+
INSERT INTO transaction VALUES(1, 1, '2022-03-28 12:32:22', 'DEPOSIT',  20.5, 11111111, 'aaa');
INSERT INTO transaction VALUES(2, 2, '2022-03-29 12:34:55', 'WITHDRAW', 8.9,  11111111, 'bbb');


-- customer_has_account
-- +------------+------+------+-----+---------+-------+
-- | Field      | Type | Null | Key | Default | Extra |
-- +------------+------+------+-----+---------+-------+
-- | customerId | int  | NO   | PRI | NULL    |       |
-- | accountId  | int  | NO   | PRI | NULL    |       |
-- +------------+------+------+-----+---------+-------+
INSERT INTO customer_has_account VALUES(1, 1);
INSERT INTO customer_has_account VALUES(1, 2);
INSERT INTO customer_has_account VALUES(2, 3);
INSERT INTO customer_has_account VALUES(2, 4);
INSERT INTO customer_has_account VALUES(3, 5);

