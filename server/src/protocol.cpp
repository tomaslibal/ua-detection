#include "protocol.h"

#include <string.h>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

vector<string>* tok(const char* msg) {
    const char s = ' ';
    char c;
    string str = string(msg);
    vector<string>* tokens = new vector<string>;
    stringstream buffer;
    for(int i = 0; i < str.length(); i++) {
        c = str.at(i);
        if (c == s) {
            tokens->push_back(buffer.str());
            buffer.str("");
        } else {
            buffer << c;
        }
    }
    
    if (buffer.str().length() > 0) {
        tokens->push_back(buffer.str());
    }
    
    return tokens;
}

string* process_message(const char* msg) {
    cout << "I got: " << msg << endl;
    
    vector<string>* tokens = tok(msg);
    
    string operation = "";
    string what = "";
    string data = "";
    
    for(int i = 0; i < tokens->size(); i++) {
        if (operation == "") {
            operation = tokens->at(i);
        } else if (what == "") {
            what = tokens->at(i);
        } else {
            data += tokens->at(i) + " ";
        }
    }
    
    delete tokens;
    
    cout << "op: " << operation << endl;
    cout << "what: " << what << endl;
    cout << "data: " << data << endl;
    
    string* output = new string("OK");
    //output->append(msg);
    return output;
}