var http        = require('http');
var Router      = require('node-simple-router');
var MongoClient = require('mongodb').MongoClient;
var ObjectID    = require('mongodb').ObjectID;

var router = Router();
var db_uri = 'mongodb://localhost:27017/ua_detection';
var stdHeadersAndCORS = {
    'Content-Type': 'application/json',
    'Access-Control-Allow-Origin': '*'
};

function getFreshOutput(id) {
    return {
        id: id,
        error: null,
        results: []
    };
};

function getCnt(coll, res) {
    MongoClient.connect(db_uri, function(err, db) {
        if(err) return;
        if(!db) return;

        // Make sure the user input is safe
        db.collection(coll).count(function(err,cnt) {
            res.writeHead(200, stdHeadersAndCORS);
            res.end(JSON.stringify({'count': cnt}));
        });
    });
};

var output = getFreshOutput();

function outFromDb(res, output, collection, query, options) {
    MongoClient.connect(db_uri, function(err, db) {
        if(err) output.error = err;
        if(!options) options = {};
        if(!db) return;

        var col = db.collection(collection);
        col.find(query, options).toArray(function(err, entries) {
            if(err) return;

            entries.forEach(function(one) {
                if(!one) return;
                output.results.push(one);
            });

            res.end(JSON.stringify(output));
        });
    });
};

router.get("/debug", function(req, res) {
    var output = { "api_status": "OK", "router": "OK"};

    MongoClient.connect(db_uri, function(err, db) {
        if(err) output.db = err;
        output.db = 'OK';

        res.writeHead(200, stdHeadersAndCORS);
        res.end(JSON.stringify(output));
    });
});

router.get('/collections', function(req, res) {
    res.writeHead(200, stdHeadersAndCORS);
    res.end(JSON.stringify({
        collections: ['phones', 'groups', 'ua_strings', 'keywords']
    }))
});

router.get("/phones", function(req, res) {
    res.writeHead(301, {'Location': 'http://' + req.headers.host + '/phones/all'});
    res.end();
});


router.get("/phones/:id", function(req, res) {
    output = getFreshOutput(req.params.id);
    var query = {};
    if (output.id === 'count') {
        getCnt('phones', res);
        return;
    }
    if (output.id.length == 24) {
        query = {_id: new ObjectID(req.params.id)};
    }
    res.writeHead(200, stdHeadersAndCORS);

    outFromDb(res, output, 'phones', query);

});

router.get("/uas", function(req, res) {});
router.get("/uas/:id", function(req, res) {
    output = getFreshOutput(req.params.id);

    res.writeHead(200, stdHeadersAndCORS);
    // function outFromDb(res, output, collection, query, options)

    var query = {};
    if (output.id === 'count') {
        getCnt('ua_strings', res);
        return;
    }
    if (output.id) {
        if (output.id.length == 24)
            query = {_id: new ObjectID(output.id)};
    }

    outFromDb(res, output, 'ua_strings', query, { limit: 10 });
});
router.get("/uas/all/:skip/:limit", function(req, res) {
    output = getFreshOutput();
    output.skip = req.params.skip;
    output.limit = req.params.limit;

    res.writeHead(200, stdHeadersAndCORS);
    // function outFromDb(res, output, collection, query, options)

    outFromDb(res, output, 'ua_strings', {}, { skip: output.skip, limit: output.limit });
});

router.get("/groups", function(req, res) {});
router.get("/groups/:id", function(req, res) {
    output = getFreshOutput(req.params.id);

    var query = {};
    if (output.id === 'count') {
        getCnt('groups', res);
        return;
    }
    if (output.id) {
        if (output.id.length == 24)
            query = {_id: new ObjectID(output.id)};
    }

    res.writeHead(200, stdHeadersAndCORS);

    outFromDb(res, output, 'groups', query);
});

var server = http.createServer(router);
server.listen(29105);
