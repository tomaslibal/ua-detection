import SimpleHTTPServer 
import SocketServer

import BaseHTTPServer
import time

class IncomingRequestHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    def do_HEAD(self):
        self.send_response(200)
        self.send_header('Content-Type', 'text/plain')
        self.end_headers()
    def do_GET(self):
        """ This method handles a GET request """
        self.send_response(200)
        self.send_header('Content-Type', 'text/html')
        self.end_headers()
        self.wfile.write('<!doctype html><html><head></head>')
        self.wfile.write('<body>Hey sup!')

PORT=8080

HTTPServer = BaseHTTPServer.HTTPServer
httpd = HTTPServer(("", PORT), IncomingRequestHandler)
print time.asctime(), "Serving HTTP on %s" % PORT

try:
    httpd.serve_forever()
except KeyboardInterrupt:
    pass

httpd.server_close()

