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
    def __init__(self, path, mimeType='text/html'):
        self.path = path
        self.mimeType = mimeType
        
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
        return 'adding a new datapoint... <meta HTTP-EQUIV="REFRESH" content="0; url=/">'

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

class RouteGET_Table_Get(RouteGeneric):
    def serve(self):
        self.status = 200
        print "GET API: get table"
        parsed = urllib.splitquery(self.req)
        query = parsed[1]
        if (not query):
            return "no arguments passed"
        args = [query]
        if ('&' in query):
            args = query.split('&')
        tableArg = args[0]
        tableArg = tableArg.split('=')
        tableName = tableArg[1]
        pg = Pg.Pg()
        offset=0
        limit=10
        table = pg.list_table(tableName, offset, limit)
        retstr = ''
        for row in table:
            retstr += '<div><span>id</span><span>{}</span><span>ua</span><span>{}</span><span><a href="/{}_edit?id={}">edit</a> | <a href="/{}_remove?id={}">remove</a></span></div>'.format(row[0], row[1], tableName, row[0], tableName, row[0])
        return retstr

"""
    Route: GET /table_plain?
    Description: returns table rows as plain text separated by new line
                 characters.
"""
class RouteGET_Table_Plain_Get(RouteGeneric):
    def serve(self):
        self.status = 200
        print "GET API: table plain get"
        params = parse_request_path(self.req)
        tableName = params[0][1]
        offset=0 # offset=params[1][1]
        limit=10 # limit=params[2][1]
        pg = Pg.Pg()
        table = pg.list_table(tableName, offset, limit)
        retstr = ''
        for row in table:
            retstr += '{} {}\n'.format(row[0], row[1])
        return retstr


def parse_request_path(req_path):
    params=[]
    parsed = urllib.splitquery(req_path)
    if (not parsed[1]):
        return params
    query_string = parsed[1]
    args=[query_string]
    if ('&' in query_string):
        args = query_string.split('&')
    for arg in args:
        if ('=' in arg):
            arg = arg.split('=')
        else:
            arg = [arg, None]
        params.append(arg)
    return params

class RouteGET_Label_Remove(RouteGeneric):
    def serve(self):
        self.status = 200
        print "GET API: remove label"
        params = parse_request_path(self.req)
        firstParam = params[0]
        pg = Pg.Pg()
        if (firstParam[0] == 'id'):
            pg.remove_label(firstParam[1])
        return 'remove label... <meta HTTP-EQUIV="REFRESH" content="0; url=/">'

class RouteGET_Datapoint_Edit(RouteGeneric):
    def serve(self):
        self.status = 200
        print "GET API: edit datapoint"
        params = parse_request_path(self.req)
        idParam = params[0]
        if (not idParam[0] == 'id'):
            return "req. not understood"
        pg = Pg.Pg()
        datapoint_labels = pg.select_row_by_key_id('datapoint_labels', 'datapoint_id', idParam[1])
        if (datapoint_labels):
            current_datapoint_label_id = datapoint_labels[2]
            current_datapoint_id = datapoint_labels[0]
        
        labels = pg.list_table('labels', 0, 10)
        labelsSelectbox = '<select name="label1"><option value="">--</option>'
        for lab in labels:
            labelsSelectbox += '<option value="{}">{}</option>'.format(lab[0], lab[1])
        labelsSelectbox += '</select>'
        datapoint = pg.select_row_by_id('datapoints', idParam[1])
        
        datapointValue = datapoint[1]
        return '<form action="/datapoints_update" method="get"><input type="hidden" name="datapoint_id" value="{}"> Datapoint<input type="text" name="value" value="{}"> Label 1 {} <button type="submit">Update</button></form>'.format(idParam[1], datapointValue, labelsSelectbox)

class RouteGET_Datapoint_Update(RouteGeneric):
    def serve(self):
        self.status = 200
        params = parse_request_path(self.req)
        idDatapoint = params[0]
        value=params[1]
        label1=params[2]
        pg = Pg.Pg()
        pg.update_datapoint_value(idDatapoint[1], urllib.unquote_plus(value[1]))
        return 'updating datapoint... <meta HTTP-EQUIV="REFRESH" content="0; url=/">'

class Router:
    @staticmethod
    def route(reqpath, matchfunc, res):
        print "user request %s" % reqpath
        route = matchfunc(reqpath)
        route.req = reqpath
        content = route.serve()
        
        httpres = HTTP.HTTPResponse(res)
        httpres.status(route.status)
        httpres.header('Content-Type', route.mimeType)
        httpres.end_headers()
        httpres.write_text(content)
 
