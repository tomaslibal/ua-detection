var MongoClient = require('mongodb').MongoClient;
var Server      = require('mongodb').Server;

var db_uri      = "mongodb://localhost:27017/ua_detection";
var db = {
    host: 'localhost',
    port: 27017,
    name: 'ua_detection'
};

var rate = 0.1;
var adj;
var re = /([\w.]+(|\/)[0-9.]+|[\w.]+)/ig;

// First implementation - "naive" algorithms
// The following is a rough sketch how this will work
//
// Go through all groups
//   for each group
//      lookup all user-agents that
//         look up user-agent's phone and group_id
//              look up all keywords from the user-agent string
//                if user-agent's group equals current group
//                   increase the weights by the rate
//                else
//                   decrease the weights by the rate
// Done.

var mongoClient = new MongoClient(new Server(db.host, db.port));
mongoClient.open(function(err, mongoClient) {
    if(err) {
        console.dir(err);
        return;
    }

    var db         = mongoClient.db('ua_detection');
    var ua_strings = db.collection('ua_strings');
    var keywords   = db.collection('keywords');

    var groups   = db.collection('groups');
    var devices  = db.collection('devices');
    var weights  = db.collection('weights');

    groups.find().toArray(function(err, g) {
        if(err) throw err;
        if(!g) return;

        g.forEach(function(eachG) {
            var gid = eachG._id;

            ua_strings.find().toArray(function(err, uas) {
                if(err) throw err;
                if(!uas) return;

                uas.forEach(function(eachUas) {
                    devices.findOne({ _id: eachUas.device_id }, function(err, p) {
                        if(err) throw err;
                        if(!p) return;

                        var kw = eachUas.ua.match(re);

                        if (kw) {
                            keywords.find({value: {$in : kw} }).each(function(err, k) {
                                if(err) console.log("Error: ", err.errmsg);
                                if(!k) return;
                                adj = (gid.equals(p.group_id)) ? parseFloat(rate) : parseFloat(-1 * rate);
                                console.log("Adjusting " + adj);
                                weights.count({group_id: p.group_id, keyword_id: k._id},function(err, count) {
                                    if (count === 0) {
                                        weights.insert({
                                            group_id: p.group_id,
                                            keyword_id: k._id,
                                            keyword: k.value,
                                            value: adj
                                        }, {w:1}, function(err, doc) {

                                        });
                                    } else {
                                        weights.update(
                                            {
                                                "keyword_id": k._id,
                                                "keyword": k.value,
                                                "group_id": p.group_id
                                            },
                                            {
                                                $inc : { 'value' : adj }
                                            },
                                            {
                                                upsert: true,
                                                w: 1
                                            },
                                            function (err, doc) {
                                                if(err) console.log("Error: " + err.errmsg);
                                                if(!doc) return;
                                            }
                                        );
                                    }
                                });
                            });
                        }
                    });
                });
            });
        });
    });
});
