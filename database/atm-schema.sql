SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

DROP SCHEMA IF EXISTS bankdb;

-- Create the bank database
CREATE SCHEMA bankdb;
USE bankdb;

-- Create the customer table
CREATE TABLE bankdb.customer (
  customerId INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
  firstName VARCHAR(45) NOT NULL,
  lastName VARCHAR(45) NOT NULL,
  address VARCHAR(45) NOT NULL,
  phone VARCHAR(45)
) ENGINE = InnoDB;

-- Create the account table
CREATE TABLE bankdb.account (
  accountId INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
  credit INT NOT NULL,
  accountNumber VARCHAR(45) NOT NULL
) ENGINE = InnoDB;

-- Create the card table
CREATE TABLE bankdb.card (
  cardId INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
  customerId INT NOT NULL,
  accountId INT NOT NULL,
  cardNumber INT NOT NULL,
  locked TINYINT NOT NULL,
  pin VARCHAR(255) NOT NULL,

  INDEX fk_card_customer1_idx (customerId ASC) VISIBLE,
  INDEX fk_card_account1_idx (accountId ASC) VISIBLE,
  CONSTRAINT fk_card_customer1
    FOREIGN KEY (customerId)
    REFERENCES bankdb.customer (customerId)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT fk_card_account1
    FOREIGN KEY (accountId)
    REFERENCES bankdb.account (accountId)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION
) ENGINE = InnoDB;

-- Create the table linking multiple customers to a single account
CREATE TABLE bankdb.customer_has_account (
  customerId INT NOT NULL,
  accountId INT NOT NULL,

  PRIMARY KEY (customerId, accountId),

  INDEX fk_customer_has_account_account1_idx (accountId ASC) VISIBLE,
  INDEX fk_customer_has_account_customer_idx (customerId ASC) VISIBLE,
  CONSTRAINT fk_customer_has_account_customer
    FOREIGN KEY (customerId)
    REFERENCES bankdb.customer (customerId)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT fk_customer_has_account_account1
    FOREIGN KEY (accountId)
    REFERENCES bankdb.account (accountId)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION
) ENGINE = InnoDB;

-- Create the transaction table
CREATE TABLE bankdb.transaction (
  transactionId INT NOT NULL AUTO_INCREMENT,
  accountId INT NOT NULL,
  timestamp DATETIME NOT NULL,
  toAccount VARCHAR(45),
  type ENUM('INSERT', 'TAKE') NOT NULL,
  value INT NOT NULL,
  cardNumber VARCHAR(16),

  PRIMARY KEY (transactionId, accountId),

  INDEX fk_transaction_account1_idx (accountId ASC) VISIBLE,
  CONSTRAINT fk_transaction_account1
    FOREIGN KEY (accountId)
    REFERENCES bankdb.account (accountId)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION
) ENGINE = InnoDB;

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
