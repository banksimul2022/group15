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

### Build using Qt Creator

Just open the `atm-app.pro` file at the root of the repo and hit run.

### Using `qmake` from the terminal

#### Linux

```sh
mkdir group15/build
cd group15/build
qmake ../atm-app.pro && make
cp -P atm-dll/*/*.so* atm-ui/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./atm-ui ./atm-ui/atm-ui
```

#### Windows

Run these commands using the Qt provided MinGW CMD shortcut. (Search for MinGW)

```cmd
mkdir group15\build
cd group15\build
qmake -spec win32-g++ ../atm-app.pro
mingw32-make
FOR /R atm-dll %f IN (*.dll) DO COPY /Y %f atm-ui\release
atm-ui\release\atm-ui.exe
```
