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
        ret=[]
        for record in cur:
            ret.append(record)
            print record
        cur.close()
        return ret
    def select_row_by_id(self, table_name, rowId):
        cur = self.conn.cursor()
        query = 'SELECT * FROM "{}" WHERE id = {} LIMIT 1'.format(table_name, rowId)
        cur.execute(query)
        ret = cur
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
    """
/       data file is a tab separated file with two columns: label; user-agent-string
    """
    def generate_data_file(self):
        pass
        
    def __exit__(self, exc_type, exc_value, traceback):
        self.conn.close()
