""" httpend.py

    HTTP End-point for label lab (py-mgr) which interacts between the user and
    the persistent storage of users agents and their labels persisted in 
    a postgres database
"""

import BaseHTTPServer
import time

       
"""
    Base class for HTTP Request Routing
    
    A route instance is an abstract layer to a HTTP request to a certain
    location, e.g. HTTP request for /profile/123 might be server by a route
    object RouteGeneric('/profile/:id')
"""
class RouteGeneric:
    def __init__(self, path):
        self.path = path
        
    def matches(self, path):
        return self.path is path
    
    def serve(self):
        pass
       
       
"""
    A route that serves content of a file
    
    <example>
        error404 = RouteFile("path/to/404.html")
        # serves the contents of the file:
        error404.serve() 
    </example>
"""
class RouteFile(RouteGeneric):
    def serve(self):
        print "serving a file %s..." % self.path
        return open(self.path).read()
        

routes = {
    "*": RouteFile("static/404.html")
}


def findroutematch(path):
    return routes["*"]


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
        print "user request %s" % reqpath
        route = findroutematch(reqpath)
        content = route.serve()
        
        httpres = HTTPResponse(res)
        httpres.status(404)
        httpres.header('Content-Type', 'text/html')
        httpres.end_headers()
        httpres.write_text(content)
       
                
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

