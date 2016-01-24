""" httpend.py

    HTTP End-point for label lab (py-mgr) which interacts between the user and
    the persistent storage of users agents and their labels persisted in 
    a postgres database
"""

import BaseHTTPServer
import time 
import src.Routing as Routing
import re
import sys

routes = {
    "/": Routing.RouteFile("static/index.html"),
    "/app.js": Routing.RouteFile("js-dist/out.js", "application/javascript"),
    "error404": Routing.RouteErrorFile("static/404.html"),
    "/add.html": Routing.RouteFile("static/add.html"),
    "/add\?.*": Routing.RouteGETAPI("add"),
    "/label_add\?.*": Routing.RouteGET_Label_Add("add"),
    "/table\?.*": Routing.RouteGET_Table_Get("table"),
    "/labels_remove\?.*": Routing.RouteGET_Label_Remove("remove")
}

def findroutematch(path):
    for r in routes:
        regex = r + '$'
        try:
            test = re.compile(regex, re.IGNORECASE)
            match = test.match(path)
            if (match):
                return routes[r]
        except:
            e = sys.exc_info()[0]
            print e
    return routes["error404"]

             
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

