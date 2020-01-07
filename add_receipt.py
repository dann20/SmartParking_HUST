import sqlite3
from find_user import find_user_id
import os
def insertReceipt(id, lot_no, enter_time, exit_time, username, fee):
    try:
        sqliteConnection = sqlite3.connect('db.sqlite3')
        cursor = sqliteConnection.cursor()
        sqlite_insert_with_param = """INSERT INTO receipts_receipt
                          (id, lot_no, enter_time, exit_time, owner_id, fee) 
                          VALUES (?, ?, ?, ?, ?, ?);"""

        data_tuple = (id, lot_no, enter_time, exit_time, find_user_id(username), fee)
        cursor.execute(sqlite_insert_with_param, data_tuple)
        sqliteConnection.commit()
        cursor.close()
        os.system("python manage.py makemigrations")
        os.system("python manage.py migrate")

    except sqlite3.Error as error:
        print(error)
    finally:
        if (sqliteConnection):
            sqliteConnection.close()

def count_receipt():
    try:
        sqliteConnection = sqlite3.connect('db.sqlite3')
        cursor = sqliteConnection.cursor()
        count = """SELECT * FROM receipts_receipt"""
        cursor.execute(count)
        count = cursor.fetchall()
        id1 = 1
        for row in count:
            id1 += 1
        cursor.close()
    except sqlite3.Error as error:
        print(error)
    finally:
        if (sqliteConnection):
            sqliteConnection.close()
        return str(id1)


def replace_receipt(username, enter_time, exit_time, fee):
    try:
        sqliteConnection = sqlite3.connect('db.sqlite3')
        cursor = sqliteConnection.cursor()
        sql_select_query = """SELECT * FROM receipts_receipt WHERE owner_id = ?"""
        cursor.execute(sql_select_query, (find_user_id(username),))
        records = cursor.fetchall()
        id = 0
        lot_no = 0
        owner_id = 0
        for receipt in records:
            if receipt[2] == enter_time:
                id = receipt[0] - 1
                lot_no = receipt[1]
                owner_id = receipt[4]
       
        sql_select_query = """DELETE FROM receipts_receipt WHERE owner_id = ? AND enter_time = ?"""
        cursor.execute(sql_select_query, (find_user_id(username), enter_time, ) )
        sqliteConnection.commit()
        cursor.close()
        err = True
        while err:
            try:
                id += 1
                insertReceipt(id, lot_no, enter_time, exit_time, find_user_id(username), fee)
                
                err = False
            except:
                err = True
    except sqlite3.Error as error:
        print(error)

    finally:
        if (sqliteConnection):
            sqliteConnection.close()
sqliteConnection = sqlite3.connect('db.sqlite3')
cursor = sqliteConnection.cursor()

sql_select_query = """DELETE FROM receipts_receipt WHERE lot_no =  ?"""
cursor.execute(sql_select_query, ('3',) )
sqliteConnection.commit()
cursor.close()
