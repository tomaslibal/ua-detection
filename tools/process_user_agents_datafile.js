//
// This tool automatically imports a large number of User-Agent strings into
// the mongodb.
//
// Data input: text file UTF-8 encoded with each User-Agent on a separate line.
// First twenty columns in that file are reserved for future use. The first
// character of the User-Agent string should start at column 20.
//
// Process:
// Connect to the mongodb, open the file, and read it line by line
//     Trim the first 20 characters from the line
//     Insert into/Update the User-Agent strings document with the User-Agent string
//     Split the User-Agent into individual keywords
//         Insert into/update the keywords document with the current keyword
//
// @version 0.4.0 changed from mongojs to the mongodb driver
//
// Invokation:
//     > cd /path/to/ua_detection/tools/
//     > node process_user_agents_datafile.js
//
// And wait until the program prints 'ALL DONE!'
//

// include the required packages
var fs          = require('fs');
var readline    = require('readline');
var stream      = require('stream');
var querystring = require("querystring");
var MongoClient = require('mongodb').MongoClient;
var Server      = require('mongodb').Server;

// processing configuration
var data_file   = '../data/user_agents.txt';
var LIMIT_ROWS  = 200000;
var current_row = 1;
var uas_array   = [];
var db = {
    host: 'localhost',
    port: 27017,
    name: 'ua_detection'
};

console.log("Starting...");

var mongoClient = new MongoClient(new Server(db.host, db.port));
mongoClient.open(function(err, mongoClient) {
    if(err) {
        console.dir(err);
        return;
    }

    var instream    = fs.createReadStream(data_file);
    var outstream   = new stream;
    var rl          = readline.createInterface({
        input: instream,
        output: process.stdout,
        terminal: false
    });

    var ua_detection = mongoClient.db(db.name);
    var uas          = ua_detection.collection('ua_strings');
    var keywords     = ua_detection.collection('keywords');

    rl.on('line', function(line) {
        if (LIMIT_ROWS === current_row) {
            console.log("Reached LIMIT_ROWS, closing the ReadStream...");
            rl.close();
            return;
        }

        // Cut-off the first two columns, leave only the user-agent strings
        // That's because the datafile has this structure:
        // 000000001 000000001 User-Agent starts on the 20th column...
        var clean = line.substr(20);

        uas_array.push({
            ua: clean,
            device_id: null,
            confidence: 0
        });

        // Decompose the UAS into keywords
        // From the HTTP/1.1 definition the User-Agent string is a collection of
        // 1) product names in the form of
        //     Product-name ["/" product-version]
        // 2) or the comment keywords closed in parenthesses whose content is
        // any text
        // The product names should be listed in the order of importance from the
        // most important down to the least important.

        var re = /([\w.]+(|\/)[0-9.]+|[\w.]+)/ig;

        var keyw = clean.match(re);
        if (keyw) {
            console.log("Processing the keywords now");
            for (var i = 0, max = keyw.length; i < max; i += 1) {
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
                    upsert: true,
                    w: 1
                },
                function(err, doc) {});
            }
        }

        console.log(current_row);
        current_row += 1;
    });

    rl.on('close', function() {
        uas.insert(uas_array, {w:1}, function(err, result) {
            mongoClient.close();
            console.log("ALL DONE!");
            process.exit(0);
        });

        console.log("User-agent Strings Loaded");
    });

});
