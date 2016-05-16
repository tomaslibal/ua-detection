#include "./uadet2.h"

#include <iostream>
#include <sstream>

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

/**
 * Splits a string by given delimiter and returns a vector of string tokens
 * @param std::string& The string to be split
 * @param char A character delimiter
 * @param std::vector<std::string>& Vector container for the split tokens
 * @returns void
 */
void strsplit(const std::string& str, char delimiter, std::vector<std::string>& tokens) {
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
}

/*
 * Prints the error message and exits the program with the implementation
 * dependent EXIT_FAILURE exit code.
 */
void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}