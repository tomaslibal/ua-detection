import psycopg2

class Pg:
    dbname = 'uadet'
    user = 'postgres'
    conn = None
    def __init__(self):
        self.conn = psycopg2.connect("dbname=" + self.dbname + " user=" + self.user)
    def list_table(self, table_name, offset, limit):
        cur = self.conn.execute("SELECT * FROM %s LIMIT %s OFFSET %s", (table_name, limit, offset))
        cur.fetchone()
        cur.close()
    def __exit__(self, exc_type, exc_value, traceback):
        self.conn.close()
