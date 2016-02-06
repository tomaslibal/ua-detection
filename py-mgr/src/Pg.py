import psycopg2

"""
    PgConnection handles connection to Postgres database as well as
    the execution of queries.

    This class is intended for database access objects which would
    model the queries needed for their purposes and then call methods
    on this class to persist changes to the database.
"""
class PgConnection:
    dbname = 'uadet'
    user = 'client'
    pwd = 'client'
    conn = None
    def __init__(self):
        pass
    def connect(self):
        self.conn = psycopg2.connect(
            dbname=self.dbname,
            user=self.user,
            password=self.pwd,
            host="127.0.0.1"
        )
    def close(self):
        if (self.conn):
            self.conn.close()
    def commit(self):
        if (self.conn):
            self.conn.commit()
    def __exit__(self, exc_type, exc_value, traceback):
        self.close()

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
        ret=[]
        for record in cur:
            ret.append(record)
            print record
        cur.close()
        return ret
    def select_row_by_id(self, table_name, rowId):
        return self.select_row_by_key_id(table_name, 'id', rowId)
    def select_row_by_key_id(self, table_name, key, keyId):
        cur = self.conn.cursor()
        query = 'SELECT * FROM "{}" WHERE {} = {} LIMIT 1'.format(table_name, key, keyId)
        cur.execute(query)
        ret = cur.fetchone()
        cur.close()
        return ret
    def add_datapoint(self, datapoint_value):
        cur = self.conn.cursor()
        query = 'INSERT INTO datapoints (value) VALUES(\'{}\')'.format(datapoint_value)
        cur.execute(query)
        self.conn.commit()
        cur.close()
    def add_label(self, label_value):
        cur = self.conn.cursor()
        query = 'INSERT INTO labels (value) VALUES(\'{}\')'.format(label_value)
        cur.execute(query)
        self.conn.commit()
        cur.close()
    def remove_label(self, label_id):
        cur = self.conn.cursor()
        query = 'DELETE FROM labels WHERE id = \'{}\''.format(label_id)
        cur.execute(query)
        self.conn.commit()
        cur.close()
    def add_datapoint_label(self, datapoint_id, label_id, confidence=0):
        cur = self.conn.cursor()
        query = 'INSERT INTO datapoint_labels (datapoint_id, label_id, confidence) VALUES({}, {}, {})'.format(datapoint_id, label_id, confidence)
        cur.execute(query)
        self.conn.commit()
        cur.close()
    def update_datapoint_value(self, datapoint_id, datapoint_value):
        cur = self.conn.cursor()
        query = 'UPDATE datapoints SET value = \'{}\' WHERE id = {}'.format(datapoint_value, datapoint_id)
        cur.execute(query)
        self.conn.commit()
        cur.close()
    def get_label_id_by_val(self, label_value):
        cur = self.conn.cursor()
        query = 'SELECT id FROM labels WHERE value = \'{}\''.format(label_value)
        cur.execute(query)
        result=cur.fetchone()
        cur.close()
        return result
    def update_datapoint_label(self, datapoint_label_id, label_id):
        pass
    """
/       data file is a tab separated file with two columns: label; user-agent-string
    """
    def generate_data_file(self):
        pass
        
    def __exit__(self, exc_type, exc_value, traceback):
        self.conn.close()
