var MongoClient = require('mongodb').MongoClient;
var ObjectId    = require('mongodb').ObjectID;
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
// Go through all devices
//   for each device as device_item
//      lookup all user-agents
//         look up user-agent's device_id
//              look up all keywords from the user-agent string
//                if user-agent's device_id equals device_item._id
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
    //var weights  = db.collection('weights');

    // WEIGHTS - DEVICES
    devices.find().toArray(function(err, entries) {
        if(err) {}

        entries.forEach(function(device_item) {
            var item_id = device_item._id;

            ua_strings.find().toArray(function(err, uas) {
                if(err) {}

                uas.forEach(function(ua) {
                    var device_id = ua.device_id;
                    var kws = ua.ua.match(re);

                    if(!device_id) { return; }

                    adj = (device_id.equals(item_id)) ? rate : (-1*rate);

                    for(var i = 0; i < kws.length; i += 1) {

                        var inc = { $inc : {} };
                        inc.$inc['devices_weights.' + item_id.toString()] = adj;
                        keywords.update(
                            { value:kws[i] },
                            inc,
                            {w:1},
                            function(err, result){}
                        );
                    }

                });
            });
        });
    });

    // WEIGHTS - GROUPS
    groups.find().toArray(function(err, entries) {
        if(err) {}

        entries.forEach(function(group_item) {
            var item_id = group_item._id;

            ua_strings.find().toArray(function(err, uas) {
                if(err) {}

                uas.forEach(function(ua) {
                    var group_id = ua.group_id;
                    var kws = ua.ua.match(re);

                    if(!group_id) { return; }

                    adj = (group_id.equals(item_id)) ? rate : (-1*rate);

                    for(var i = 0; i < kws.length; i += 1) {

                        var inc = { $inc : {} };
                        inc.$inc['groups_weights.' + item_id.toString()] = adj;
                        keywords.update(
                            { value:kws[i] },
                            inc,
                            {w:1},
                            function(err, result){}
                        );
                    }

                });
            });
        });
    });
});
