var MongoClient = require('mongodb').MongoClient;
var Server      = require('mongodb').Server;

var db = {
    host: '127.0.0.1',
    port: 27017,
    db_name: 'ua_detection'
};

// read each document from the 'training_set' collection
// for each document
// do
//     init _id, ua_string_id, group_id, vector_calculated, input_vector, expected_output
//     if vector_calculated = 1 -> continue
//     // decouple keywords of the user_string
//     sample_mean = 0
//     i       = 0
//     sum     = 0
//     weights = []
//     keywords = regex(re, db.ua_strings.findOne(ua_string_id))
//     for each keyword
//     do
//         weights[i] = weights.findOne(keyword_id: <keyword._id>, group_id: <group_id>)
//         sum += weights[i]
//         i++
//     end
//     sample_mean = sum / i
//     for each weights[]
//     i = 0
//     do
//         P += (weights[i] - sample_mean)^2
//         i++
//     end
//     std_dev = sqrt(P/i)
//     ua_str_len = strlen(ua_string)
//     if (ua_string_id->device->group_id == group_id) exp = 1 else exp = 0
//     db.training_set.update({_id: <id>},
//                     { input_vector: [sample_mean, std_dev, ua_str_len],
//                       expected_output: exp,
//                       vector_calculated: 1 }, {w:1}, writeconcerncallback );
// end
//
// Training set input vector calculation and expected_value assignment done

var mongoClient = new MongoClient(new Server('localhost', 27017));
mongoClient.open(function(err, mongoClient) {
    var db = mongoClient.db("ua_detection");

    // populate training_set from ua_strings which have assigned a device_id or a group_id
    //
    //

    db.collection('training_set').find().toArray(function(err, rows) {
        if(err) throw err;

        // For each document:
        rows.forEach(function(row) {
            if(!row) return;
            if (1 === row.vector_calculated) return;

            var sample_mean = 0;
            var std_dev = 0;
            var strlen  = 0;
            var exp     = 0;

            var group_id = row.group_id;
            var device_id = row.device_id;

            strlen = row.ua_string.length;

            var re = /([\w.]+(|\/)[0-9.]+|[\w.]+)/ig;
            var keywords = row.ua_string.match(re);
            db.collection('keywords').find({ value: { $in : keywords } }).toArray(function(err, kws) {
                if(err) throw err;

                var kws_ids = [];
                var i       = 0;
                var sum     = 0;
                var P       = 0;

                kws.forEach(function(k) {
                    kws_ids.push(k._id);

                    sum += k.devices_weights[device_id];
                    i   += 1;
                });

                sample_mean = sum / i;

                kws.forEach(function(k) {
                    P += Math.pow((k.devices_weights[device_id] - sample_mean), 2);
                });

                std_dev = Math.sqrt(P/(i-1));

                db.collection('ua_strings').findOne({_id: row.ua_string_id }, function(err, ua) {
                    if(err) throw err;

                    if(ua.device_id.equals(device_id))
                        exp = 1;
                    else
                        exp = 0;

                    row.input_vector = [ sample_mean, std_dev, strlen];
                    row.expected_output = exp;
                    row.vector_calculated = 1;
                    db.collection('training_set').update({
                        _id: row._id
                    },row, {
                        w: 1
                    }, function(err, d){});

                });

                // db.collection('weights').find({ group_id: row.group_id, keyword_id: { $in : kws_ids }}).toArray(function(err, w) {
                //     if(err) throw err;
                //
                //     w.forEach(function(weight) {
                //         sum += weight.value;
                //         i++;
                //     });
                //
                //     sample_mean = sum / i;
                //
                //     var P = 0;
                //     //i = 0;
                //
                //     w.forEach(function(weight) {
                //         P += Math.pow((weight.value - sample_mean), 2);
                //         //i++;
                //     });
                //     std_dev = Math.sqrt(P/(i-1));
                //
                //     db.collection('ua_strings').findOne({_id: row.ua_string_id }, function(err, ua) {
                //         if(err) throw err;
                //
                //         db.collection('devices').findOne({ _id : ua.device_id }, function(err, device) {
                //             if(err) throw err;
                //
                //             if(device.group_id.equals(group_id))
                //                 exp = 1;
                //             else
                //                 exp = 0;
                //
                //             row.input_vector = [ sample_mean, std_dev, strlen];
                //             row.expected_output = exp;
                //             row.vector_calculated = 1;
                //             db.collection('training_set').update({
                //                 _id: row._id
                //             },row, {
                //                 w: 1
                //             }, function(err, d){});
                //         });
                //     });
                // });
            });

        });
    });
});
