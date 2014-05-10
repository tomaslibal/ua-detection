// $tot = count($keywords)
// foreach $keywords as $key
// do
//     $occ = $key.count / $tot
//     $pos = $key.avg_position
//     $input_vector = (1, occ, pos)
//     $exp = 0 // this will need to be set manually in absence of a table of product keywords. Exp=1 -> unique product keyword, Exp=0 any other keyword
//     $training_set.insert({$key, $input_vector, $exp})
// end

var MongoClient = require('mongodb').MongoClient;
var Server      = require('mongodb').Server;

var db = {
    host: 'localhost',
    port: 27017,
    name: 'ua_detection'
};

var mongoClient = new MongoClient(new Server(db.host, db.port));
mongoClient.open(function(err, mongoClient) {
    if(err) {
        console.dir(err);
        return;
    }

    var ua_detection = mongoClient.db(db.name);
    var uas          = ua_detection.collection('ua_strings');
    var keywords     = ua_detection.collection('keywords');
    var set          = ua_detection.collection('keywords_training_set');

    var kall = [];

    keywords.count(function(err, count) {
        var cnt = count;

        keywords.find().toArray(function(err, all) {
            all.forEach(function(k) {
                var occ = k.count/cnt;
                var tmp = {
                    keyword: k.value,
                    keyword_id: k._id,
                    input_vector: [1.0, occ, k.avg_position],
                    expected_output: 0,
                    vector_calculated: 1
                };
                kall.push(tmp);
            });

            set.insert(kall, {w:1}, function(err,res){
                if(!err) {
                    console.log("Done");
                    mongoClient.close();
                    process.exit(0);
                }
            });
        });
    });
});
