# group15 - ATM project setup guide

## atm-api

### database
Begin by installing `mysql/mariadb` on the server.  
Then run the following commands at the root of the git repo.

```sh
mysql -u[user] -p[password] < database/atm-schema.sql
# And optionally load the test data
mysql -u[user] -p[password] < database/test-data.sql
```

### nodejs
Next install nodejs on the server.  
The run the following commands in the `atm-api` directory of the repo.

```sh
# Install node dependecies
npm install
# Generate the JWT_TOKEN (You need to use git bash or some other unix terminal emulator on windows for this)
./gen_token.sh
# Set the SQL_USER and SQL_PASSWORD vars in the .env file to your databases user and password
# Then finally run the server with the following command
npm start
```

## atm-ui

Begin by opening each project in the `atm-dll` directory using Qt Creator and build them.  
Finally you can open the main `atm-ui` project and build that and then run it.
