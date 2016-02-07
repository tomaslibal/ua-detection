import HTTP
import urllib
import Pg
import json

"""
    Routing.py defines actions for various routes. A route is a mapping
    from an HTTP request's path (e.g. /some/path/0/1/) to these classes
    done by the matching function in httpend.py.

    The idea is that these routes build up an API structure for this 
    Python HTTP server.

    Most transported data should be text/json format. This needs some 
    updates.
"""

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
    Route: <filename>
    Description: A route that serves content of a file
    
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

"""
    Route: <error case>
    Description: Used for 404 Not Found error.
"""
class RouteErrorFile(RouteFile):
    def serve(self):
        self.status = 404
        print "serving an error file %s..." % self.path
        return open(self.path).read()

"""
    Route: GET /datapoint_add?
    Description: Takes the value of the new datapoint and calls Pg class
                 to store this value in DB.
"""
class RouteGETAPI(RouteGeneric):
    def serve(self):
        self.status = 200
        print "serving GET request to the API for action %s" % self.path
        parsed = urllib.splitquery(self.req)
        params = parse_request_path(self.req)
        datapoint=params[0]
        label1=params[1]
        pg = Pg.Pg()
        newId=None
        if (datapoint[0] == "dp"):
            newId=pg.add_datapoint(urllib.unquote_plus(datapoint[1]))
        if (label1[0] == "cat1" and newId is not None):
            label1Id = pg.get_label_id_by_val(label1[1])
            pg.add_datapoint_label(newId, label1Id[0])
        return 'adding a new datapoint... <meta HTTP-EQUIV="REFRESH" content="0; url=/">'

"""
    Route: GET /label_add?
    Description: Takes an argument which is the label value and calls
                 a method on Pg class to store that label in DB.
"""
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

"""
    Route: GET /table?
    Description: LEGACY route which lists table rows as formatted HTML
                 string.
"""
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
        # table = pg.get_datapoints_with_labels(offset, limit)
        retstr = ''
        for row in table:
            label1=''
            if (tableName == 'datapoints'):
                labels = pg.get_datapoint_labels(row[0])
                if (labels):
                    label1 = 'labels: ' + labels[0][0]
            retstr += '<div><span>id</span><span>{}</span><span>value</span><span>{}</span><span>{}</span><span><a href="/{}_edit?id={}">edit</a> | <a href="/{}_remove?id={}">remove</a></span></div>'.format(row[0], row[1], label1, tableName, row[0], tableName, row[0])
        return retstr

"""
    Route: GET /table_plain?
    Description: returns table rows as text/json.
"""
class RouteGET_Table_Get_Json(RouteGeneric):
    def serve(self):
        self.status = 200
        self.mimeType='text/json'
        print "GET API: table plain get"
        params = parse_request_path(self.req)
        tableName = params[0][1]
        offset=0 # offset=params[1][1]
        limit=10 # limit=params[2][1]
        pg = Pg.Pg()
        table = pg.list_table(tableName, offset, limit)
        retstr = '{ rows: ['
        for row in table:
            retstr += '{{ rowId: {0}, value: "{1}" }}\n'.format(row[0], row[1])
        return retstr+'] }';


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

class RouteGET_Datapoint_Remove(RouteGeneric):
    def serve(self):
        self.status=200
        params = parse_request_path(self.req)
        if (params[0] != "id"):
           return "error. no id supplied"
        idDatapoint = params[0][0]
        pg.remove_datapoint(idDatapoint)
        return 'Removing datapoint... <meta http-equiv="refresh" content="0; url=/">'

class RouteGET_Generate_Download(RouteGeneric):
    def serve(self):
        self.status = 200
        pg = Pg.Pg()
        rows = pg.get_datapoints_with_first_label()
        f = open('out/data_down.txt', 'w')
        for row in rows:
            f.write('{}\t{}\n'.format(row[0], row[1]))
        return 'generating the file... <meta http-equiv="refresh" content="0; url=/download_list.html">'

class RouteGET_Download_List(RouteGeneric):
    def serve(self):
        self.status = 200
        return '<h1>Download list</h1><a href="/data_down.txt">Datapoints with first label</a>'




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
 
