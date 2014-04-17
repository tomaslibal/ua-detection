// include required packages
var fs       = require('fs');
var readline = require('readline');
var stream   = require('stream');

// processing configuration
var data_file   = '../data/user_agents.txt';
var LIMIT_ROWS  = 200000;
var current_row = 1;

var instream    = fs.createReadStream(data_file);
var outstream   = new stream;
var rl          = readline.createInterface(instream, outstream);

var querystring = require("querystring");

var db_uri      = "mongodb://localhost/ua_detection";
var MongoClient = require('mongodb').MongoClient;

MongoClient.connect(db_uri, function(err, db) {
    if(err) console.log(err);
    if(!db) return;

    var uas = db.collection('ua_strings');
    var keywords = db.collection('keywords');

    rl.on('line', function(line) {
        if (LIMIT_ROWS === current_row) {
            rl.close();
            return;
        }
        // Cut-off the first two columns, leave only the user-agent strings
        // That's because the datafile has this structure:
        // 000000001 000000001 User-Agent starts on the 20th column...
        var clean = line.substr(20);

        uas.update({
            ua: clean,
            phone_id: null,
            confidence: 0,
        }, {
            $inc: { cnt: 1 }
        },
        {
            upsert: true
        },
        function(err, doc){});

        // Decompose the UAS into keywords
        // Product-name "/" product-version
        var re = /([\w.]+(|\/)[0-9.]+|[\w.]+)/ig;

        var keyw = clean.match(re);
        console.log(keyw);
        if (keyw) {
            console.log("Processing the keywords now");
            for (var i = 0, max = keyw.length; i < max; i += 1) {
                console.log(keyw[i]);
                keywords.update(
                {
                    value: keyw[i]
                },
                {
                    $inc: {
                        count: 1
                    }
                },
                {
                    upsert: true
                },
                function(err, doc) {});
            }
        }

        console.log(current_row);
        current_row += 1;
    });

    rl.on('close', function() {
        //db.close();
        console.log("FINISHED!!!");
    });

});
