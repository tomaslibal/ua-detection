class HTTPResponse:
    def __init__(self, conn):
        self.conn = conn
        
    def status(self, code):
        self.conn.send_response(code)

    def header(self, headername, headerval):
        self.conn.send_header(headername, headerval)
        
    def end_headers(self):
        self.conn.end_headers()

    def write_text(self, text):
        self.conn.wfile.write(text)
