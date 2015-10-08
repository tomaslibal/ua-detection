""" httpend.py

    HTTP End-point for label lab (py-mgr) which interacts between the user and
    the persistent storage of users agents and their labels persisted in 
    a postgres database
"""

import BaseHTTPServer
import time

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


class Router:
    @staticmethod
    def route(reqpath, res):
        print reqpath
        httpres = HTTPResponse(res)
        httpres.status(200)
        httpres.header('Content-Type', 'text/html')
        httpres.end_headers()
        httpres.write_text('<!doctype html><html><head><title>Response</title></head><body>')
        httpres.write_text('<p>You requested %s</p>' % reqpath)
        
class IncomingRequestHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    def do_HEAD(self):
        self.send_response(200)
        self.send_header('Content-Type', 'text/plain')
        self.end_headers()
    def do_GET(self):
        """ This method handles a GET request """
        Router.route(self.path, self)

PORT=8080

HTTPServer = BaseHTTPServer.HTTPServer
httpd = HTTPServer(("", PORT), IncomingRequestHandler)
print time.asctime(), "Serving HTTP on %s" % PORT

try:
    httpd.serve_forever()
except KeyboardInterrupt:
    pass

httpd.server_close()

