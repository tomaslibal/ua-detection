import HTTP

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
    
    
class Router:
    @staticmethod
    def route(reqpath, res):
        print "user request %s" % reqpath
        route = findroutematch(reqpath)
        content = route.serve()
        
        httpres = HTTP.HTTPResponse(res)
        httpres.status(404)
        httpres.header('Content-Type', 'text/html')
        httpres.end_headers()
        httpres.write_text(content)
 
