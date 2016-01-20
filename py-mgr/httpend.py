""" httpend.py

    HTTP End-point for label lab (py-mgr) which interacts between the user and
    the persistent storage of users agents and their labels persisted in 
    a postgres database
"""

import BaseHTTPServer
import time 
import src.Routing as Routing


routes = {
    "/": Routing.RouteFile("static/index.html"),
    "*": Routing.RouteFile("static/404.html"),
    "/add.html": Routing.RouteFile("static/add.html")
}


def findroutematch(path):
    if path in routes:
        return routes[path]
    return routes["*"]

             
class IncomingRequestHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    def do_HEAD(self):
        self.send_response(200)
        self.send_header('Content-Type', 'text/plain')
        self.end_headers()
    def do_GET(self):
        """ This method handles a GET request """
        Routing.Router.route(self.path, findroutematch, self)

PORT=8080

HTTPServer = BaseHTTPServer.HTTPServer
httpd = HTTPServer(("", PORT), IncomingRequestHandler)
print time.asctime(), "Serving HTTP on %s" % PORT

try:
    httpd.serve_forever()
except KeyboardInterrupt:
    pass

httpd.server_close()

