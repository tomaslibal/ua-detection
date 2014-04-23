var MongoClient = require('mongodb').MongoClient;

var db_uri      = "mongodb://localhost:27017/ua_detection";

var rate = 0.1;
var adj;
var re   = /[\w.]+/g;

// First implementation - "naive" algorithms
// The following is a rough sketch how this will work

// First run:
// Go through all user-agent strings,
//    For each user-agent
//      decouple the keywords
//         for each keyword
//           insert it into database / update the cnt if keyword existed
// Second run:
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

MongoClient.connect(db_uri, function(err, db) {
    if(err) console.log(err);

    var ua_strings = db.collection('ua_strings');
    var keywords   = db.collection('keywords');

    ua_strings.find().each(function(err, ua_string) {
        if(err) throw err;
        if(!ua_string) return;

        var kw = ua_string.ua.match(re);

        if(kw) {
            for(var n = 0, max = kw.length; n < max; n += 1) {
                console.log("Updating keyword ", kw[n]);
                keywords.update(
                    { value: kw[n] },
                    { $inc: { count: 1 } },
                    { upsert: true, w: 1 },
                    function () {}
                );
            }
        }

    });

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
                                            value: adj
                                        }, {w:1}, function(err, doc) {

                                        });
                                    } else {
                                        weights.update(
                                            {
                                                "keyword_id": k._id,
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
