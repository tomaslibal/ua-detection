import HTTP
import urllib
import Pg

"""
    Base class for HTTP Request Routing
    
    A route instance is an abstract layer to a HTTP request to a certain
    location, e.g. HTTP request for /profile/123 might be server by a route
    object RouteGeneric('/profile/:id')
"""
class RouteGeneric:
    status = 404
    req = ''
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
        self.status = 200
        print "serving a file %s..." % self.path
        return open(self.path).read()

class RouteErrorFile(RouteFile):
    def serve(self):
        self.status = 404
        print "serving an error file %s..." % self.path
        return open(self.path).read()

class RouteGETAPI(RouteGeneric):
    def serve(self):
        self.status = 200
        print "serving GET request to the API for action %s" % self.path
        parsed = urllib.splitquery(self.req)
        query = parsed[1]
        if (not query):
            return "no arguments passed"
        if ('&' in query):
            args = query.split('&')
            query = args[0]
        
        datapoint = query # args[0]
        d = datapoint.split('=')
        pg = Pg.Pg()
        if (d[0] == "dp"):
            pg.add_datapoint(urllib.unquote_plus(d[1]))
        return "adding a new datapoint..."

class RouteGET_Label_Add(RouteGeneric):
    def serve(self):
        self.status = 200
        print "GET API: add new label"
        parsed = urllib.splitquery(self.req)
        query = parsed[1]
        if (not query):
            return "no arguments passed"
        if ('&' in query):
            args = query.split('&')
            query = args[0]
        label = query
        d = label.split('=')
        pg = Pg.Pg()
        if (d[0] == 'label'):
            pg.add_label(urllib.unquote_plus(d[1]))
        return 'adding a new label... <meta HTTP-EQUIV="REFRESH" content="0; url=/">'
 
class Router:
    @staticmethod
    def route(reqpath, matchfunc, res):
        print "user request %s" % reqpath
        route = matchfunc(reqpath)
        route.req = reqpath
        content = route.serve()
        
        httpres = HTTP.HTTPResponse(res)
        httpres.status(route.status)
        httpres.header('Content-Type', 'text/html')
        httpres.end_headers()
        httpres.write_text(content)
 
