-- customer
-- +------------+-------------+------+-----+---------+----------------+
-- | Field      | Type        | Null | Key | Default | Extra          |
-- +------------+-------------+------+-----+---------+----------------+
-- | customerId | int         | NO   | PRI | NULL    | auto_increment |
-- | firstName  | varchar(45) | NO   |     | NULL    |                |
-- | lastName   | varchar(45) | NO   |     | NULL    |                |
-- | address    | varchar(45) | NO   |     | NULL    |                |
-- | phone      | varchar(45) | YES  |     | NULL    |                |
-- +------------+-------------+------+-----+---------+----------------+
INSERT INTO customer VALUES(NULL, 'Roope', 'Ankka', 'Rahasäiliö 1', 10000);
INSERT INTO customer VALUES(NULL, 'Aku', 'Ankka', 'Paratiisitie 13', 43738);
INSERT INTO customer VALUES(NULL, 'Iines', 'Ankka', 'Lepolahti 8', 12464);
INSERT INTO customer VALUES(NULL, 'Hupu', 'Ankka', 'Paratiisitie 13', 43738);
INSERT INTO customer VALUES(NULL, 'Tupu', 'Ankka', 'Paratiisitie 13', 43738);
INSERT INTO customer VALUES(NULL, 'Lupu', 'Ankka', 'Paratiisitie 13', 43738);


-- account
-- +---------------+-------------+------+-----+---------+----------------+
-- | Field         | Type        | Null | Key | Default | Extra          |
-- +---------------+-------------+------+-----+---------+----------------+
-- | accountId     | int         | NO   | PRI | NULL    | auto_increment |
-- | credit        | int         | NO   |     | NULL    |                |
-- | accountNumber | varchar(45) | NO   |     | NULL    |                |
-- +---------------+-------------+------+-----+---------+----------------+
INSERT INTO account VALUES(NULL, 63457357, 111111);
INSERT INTO account VALUES(NULL, 21453421, 222222);
INSERT INTO account VALUES(NULL, -12, 333333);
INSERT INTO account VALUES(NULL, 313, 444444);
INSERT INTO account VALUES(NULL, 23456, 555555);


-- card
-- +------------+--------------+------+-----+---------+----------------+
-- | Field      | Type         | Null | Key | Default | Extra          |
-- +------------+--------------+------+-----+---------+----------------+
-- | cardId     | int          | NO   | PRI | NULL    | auto_increment |
-- | customerId | int          | NO   | MUL | NULL    |                |
-- | accountId  | int          | NO   | MUL | NULL    |                |
-- | cardNumber | int          | NO   |     | NULL    |                |
-- | locked     | tinyint      | NO   |     | NULL    |                |
-- | pin        | varchar(255) | NO   |     | NULL    |                |
-- +------------+--------------+------+-----+---------+----------------+
INSERT INTO card VALUES(NULL, 1, 1, 11111111, 0, 1234);
INSERT INTO card VALUES(NULL, 2, 2, 22222222, 0, 1234);
INSERT INTO card VALUES(NULL, 3, 3, 33333333, 0, 1234);
INSERT INTO card VALUES(NULL, 4, 4, 44444444, 0, 1234);
INSERT INTO card VALUES(NULL, 5, 5, 55555555, 0, 1234);
INSERT INTO card VALUES(NULL, 1, 2, 10101010, 0, 1234);


-- transaction
-- +---------------+-----------------------+------+-----+---------+----------------+
-- | Field         | Type                  | Null | Key | Default | Extra          |
-- +---------------+-----------------------+------+-----+---------+----------------+
-- | transactionId | int                   | NO   | PRI | NULL    | auto_increment |
-- | accountId     | int                   | NO   | PRI | NULL    |                |
-- | timestamp     | date                  | NO   |     | NULL    |                |
-- | toAccount     | varchar(45)           | YES  |     | NULL    |                |
-- | type          | enum('INSERT','TAKE') | NO   |     | NULL    |                |
-- | value         | int                   | NO   |     | NULL    |                |
-- | cardNumber    | varchar(16)           | YES  |     | NULL    |                |
-- +---------------+-----------------------+------+-----+---------+----------------+
INSERT INTO transaction VALUES(NULL, 1, "2022-03-28 12:32:22", 2, 'INSERT', 1000000, 11111111);
INSERT INTO transaction VALUES(NULL, 2, "2022-03-29 12:34:55", 3, 'INSERT', 100, 11111111);


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

