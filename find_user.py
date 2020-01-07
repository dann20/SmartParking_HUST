import sqlite3

def find_user_id(username):
    try:
        sqliteConnection = sqlite3.connect('db.sqlite3')
        cursor = sqliteConnection.cursor()
        sql_select_query = """SELECT * FROM auth_user WHERE username = ?"""
        cursor.execute(sql_select_query, (username,))
        record = cursor.fetchone()
        cursor.close()

    except sqlite3.Error as error:
        print(error)
    finally:
        if (sqliteConnection):
            sqliteConnection.close()
        return str(record[0])
