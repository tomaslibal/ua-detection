import psycopg2

class Pg:
    dbname = 'uadet'
    user = 'client'
    pwd = 'client'
    conn = None
    def __init__(self):
        self.conn = psycopg2.connect(dbname=self.dbname, user=self.user, password=self.pwd, host="127.0.0.1")
    def list_table(self, table_name, offset, limit):
        cur = self.conn.cursor()
        query = 'SELECT * FROM "{}" LIMIT {} OFFSET {}'.format(table_name, limit, offset)
        cur.execute(query)
        for record in cur:
            print record
        cur.close()
        return cur
    def __exit__(self, exc_type, exc_value, traceback):
        self.conn.close()
