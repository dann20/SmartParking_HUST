import sqlite3

def readSqliteTable():
    try:
        sqliteConnection = sqlite3.connect('db.sqlite3')
        cursor = sqliteConnection.cursor()
        print("Connected to SQLite")

        sqlite_select_query = """SELECT * from receipts_receipt"""
        cursor.execute(sqlite_select_query)
        records = cursor.fetchall()
        print("Total rows are:  ", len(records))
        print("Printing each row")
        id1 = 0
        for row in records:
            id1 += 1
            print("ID: ", row[0])
            print("Lot_No: ", row[1])
            print("Enter_Time: ", row[2])
            print("Exit_Time: ", row[3])
            print("User_ID: ", row[4])
        print(id1)
        cursor.close()

    except sqlite3.Error as error:
        print("Failed to read data from sqlite table", error)
    finally:
        if (sqliteConnection):
            sqliteConnection.close()
            print("The SQLite connection is closed")

readSqliteTable()