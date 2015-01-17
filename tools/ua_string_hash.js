/*
% hash function for User-Agent strings. This function takes an array of tokens
% from the User-Agent string and returns a hash which is a number from the range
% 0, 10000 (inclusive the boundaries).
%
% E.g.:
%
% "a b c d e" => n from <0, 1>
%
let input = [a, b, c, d, e]

% table (associative array of a hash table) with IDs that have already been
% assigned to keywords.
%
% E.g.:
%
% { keyword1: id1,
%   keyword2: id2,
%   ...
%   keywordN: idN }
%
let id = {}

% this is the hash
let hash = 0

for each input as keyword
    if keyword is not in $id
        id[keyword] = random(0, 10000)
    hash <- id[keyword]

hash = hash / (10000 * length(input))
*/

/*
    Sample implementation in node.js

*/
var keywords = ["Mozilla/5.0", "Macintosh", "Intel", "Mac", "OS", "X", "10_9_2", "AppleWebKit/573.36", "KHTML", "like", "Gecko", "Chrome/34.0.1847.131", "Safari/537.36"];
var id = {};
var hash = 0;

var i = 0;

function random(mi, ma) {
    "use strict";
    return Math.floor(Math.random() * (ma - mi + 1)) + mi;
}

function ua_string_hash(keywords) {
    "use strict";
    for(; i < keywords.length; i += 1) {
        if (!id.hasOwnProperty(keywords[i])) {
            id[keywords[i]] = random(0, 10000);
        }
        hash += id[keywords[i]];
    }

    return hash / (10000 * keywords.length);
}

console.dir(ua_string_hash(keywords));
