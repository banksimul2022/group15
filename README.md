# group15 - ATM Project

## atm-sql-schema.sql
SQL script to setup the database layout used by the ATM

## atm-api
NodeJS REST API backend for authentication/account management

## atm-engine
DLL/Shared library used for communication with the `atm-api`

## atm-ui
Allows the user to widthdraw/view their balance  
Uses the `atm-engine` shared library to fetch/update data in the SQL database
