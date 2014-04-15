var mongojs     = require("mongojs");

var db_uri      = "mongodb://localhost:27017/ua_detection";
var collections = ["phones", "ua_strings", "groups", "keywords", "weights"];
var db          = mongojs.connect(db_uri, collections);

var rate = 0.01;
var adj  = 0;
var re   = /[\w.]+/g;

// First implementation - "naive" algorithms
// The following is a rough sketch how this will work

// First run:
// Go through all user-agent strings,
//    For each user-agent
//      decouple the keywords
//         for each keyword
//           insert it into database / update the cnt if keyword existed

db.ua_strings.find().forEach(function(err, doc) {
    if(err) console.log("Error: ", err.errmsg);
    if(!doc) return;

    var keywords = doc.ua.match(re);

    if(keywords) {
        for(var n = 0, max = keywords.length; n < max; n += 1) {
            console.log("Updating keyword ", keywords[n]);
            db.keywords.update({"value": keywords[n]}, {$inc: { "count": 1 } }, { upsert: true});
        }
    }
});

// Second run:
// Go through all groups
//   for each group
//      lookup all user-agents
//         look up user-agent's phone
//            look up phone's group
//              look up all keywords from the user-agent
//                if user-agent's group equals current group
//                   increase the weights by the rate
//                else
//                   decrease the weights by the rate
// Done.

db.groups.find().forEach(function(err, groups) {
    if(err) console.log("Error: ", err.errmsg);
    if(!groups) return;

    console.log("Group: ", groups.name);
    console.log("Now looping through all user-agent strings");

    db.ua_strings.find().forEach(function(err, ua) {
        if(err) console.log("Error: ", err.errmsg);
        if(!ua) return;

        console.log("User-Agent: ", ua.ua);

        db.phones.findOne({"_id": ua.phone_id}, function(err, phone) {
            if(err) console.log("Error: ", err.errmsg);
            if(!phone) return;

            console.log("Phone: ", phone.name);

            db.groups.findOne({"_id": phone.group_id}, function(err, group) {
                if(err) console.log("Error: ", err.errmsg);
                if(!group) return;

                console.log("Phone's group: ", group.name);

                var keywords = ua.ua.match(re);

                console.log("Keywords in the user-agent: ", keywords);

                if (keywords) {
                    for(var n = 0, max = keywords.length; n < max; n += 1) {
                        db.keywords.findOne({"value": keywords[n]}, function(err, k) {
                            if(err) console.log("Error: ", err.errmsg);
                            if(!k) return;

                            adj = (groups._id.equals(group._id)) ? rate : -1 * rate;
                            db.weights.update({"keyword_id": k._id, "group_id": group._id}, {$inc : { value: adj } }, {upsert: true});
                        });
                    }
                }
            });
        });
    });
});
