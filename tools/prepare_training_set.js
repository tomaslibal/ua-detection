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
//     //decouple keywords of the user_string
//     ar_mean = 0
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
//     ar_mean = sum / i
//     for each weights[]
//     i = 0
//     do
//         P += (weights[i] - ar_mean)^2
//         i++
//     end
//     std_dev = sqrt(P/i)
//     ua_str_len = strlen(ua_string)
//     if (ua_string_id->device->group_id == group_id) exp = 1 else exp = 0
//     db.training_set.update({_id: <id>},
//                     { input_vector: [ar_mean, std_dev, ua_str_len],
//                       expected_output: exp }, {w:1}, writeconcerncallback );
// end
//
// Training set input vector calculation and expected_value assignment done
