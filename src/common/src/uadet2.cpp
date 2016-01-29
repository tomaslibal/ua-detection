#include "./uadet2.h"

#include <iostream>

using namespace std;

void foreach(vector<string>* vec, function<void (string)>& callback) {
    for(vector<string>::iterator it = vec->begin(); it != vec->end(); ++it) {
        callback(*it);
    }
}

void print_usage() {
    char tab = '\t';
    
    cout << "uadet2 User-agent String Classifier" << endl << endl;
    cout << tab << "uadet2 --ua='Mozilla/5.0 ...'" << endl;
    // cout << tab << "uadet2 --ua='Mozilla/5.0 ...' --category=mobile" << endl;
}

void parse_args(int argc, char** argv, UadetSettings& settings) {
    // user-agent
    string ua_arg;
    ua_arg = argv[1];
    
    string::size_type n = ua_arg.find("--ua=");
    if (n != string::npos) {
        settings.ua = ua_arg.substr(n+1);
    } else {
        throw InvalidArgsException("Cannot parse out the user-agent string");
    }
}