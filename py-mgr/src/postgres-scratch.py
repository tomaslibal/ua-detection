import psycopg2

conn = psycopg2.connect("dbname=labelling user=bar1 password=foo")

try:
    cur = conn.cursor()
    cur.execute("SELECT * FROM datapoints")
    print cur.fetchone()

    cur.close()
except Exception, ex:
    print 'exception: ' + ex.message

conn.close()
