#include "protocol.h"

#include <string.h>
#include <string>
#include <sstream>
#include <iostream>

using std::string;
using std::vector;
using std::stringstream;
using std::cout;
using std::endl;

/*
 * Tokenizes the given message of chars by splitting it into tokens.
 */
vector<string> tok(const char* msg) {
    const char s = ' ';
    char c;
    string str = string(msg);
    vector<string> tokens;
    stringstream buffer;
    for(unsigned int i = 0; i < str.length(); i++) {
        c = str.at(i);
        if (c == s) {
            tokens.push_back(buffer.str());
            buffer.str("");
        } else {
            buffer << c;
        }
    }
    
    if (buffer.str().length() > 0) {
        tokens.push_back(buffer.str());
    }
    
    return tokens;
}

/*
 * Takes an incoming message of chars and acts on it, based on the contents
 * of the message.
 */
vector<string> process_message(const char* msg) {
    cout << "I got: " << msg << endl;
    
    vector<string> tokens = tok(msg);
    
    string operation = "";
    string what = "";
    string data = "";
    
    for(unsigned int i = 0; i < tokens.size(); i++) {
        if (operation == "") {
            operation = tokens.at(i);
        } else if (what == "") {
            what = tokens.at(i);
        } else {
            data += tokens.at(i) + " ";
        }
    }
    
    cout << "op: " << operation << endl;
    cout << "what: " << what << endl;
    cout << "data: " << data << endl;
    
    vector<string> output;
    output.push_back(operation);
    output.push_back(what);
    output.push_back(data);

    return output;
}