#!/usr/bin/env python3
from random import randrange
from faker import Faker
import mysql.connector
import sys

fake = Faker()

db = mysql.connector.connect(
    host="localhost",
    user="bankuser",
    password="bankpass",
    database="bankdb"
)

cursor = db.cursor()

COUNT = 500000

for i in range(COUNT):
    sys.stdout.write(f"\r{i}/{COUNT}")
    sys.stdout.flush()
    cursor.execute(
        "INSERT INTO transaction VALUES (NULL, %s, %s, %s, %s, %s, %s)",
        (
            randrange(1, 6), 
            fake.date_time_between(start_date="-30y", end_date="now"), 
            "DEPOSIT" if randrange(2) else "WITHDRAW",
            randrange(999999) if randrange(2) else (randrange(999999) + randrange(1, 100) * 0.01),
            None if randrange(2) else ''.join(chr(randrange(65, 91)) for x in range(randrange(1, 11))),
            None if randrange(2) else ''.join(chr(randrange(65, 91)) for x in range(randrange(1, 46)))
        )
    )

print("\nCommit data...")

db.commit()

print("Done")

db.close()
