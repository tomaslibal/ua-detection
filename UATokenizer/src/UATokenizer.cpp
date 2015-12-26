/* 
 * File:   UATokenizer.cpp
 * Author: tlibal
 * 
 * Created on November 22, 2015, 7:08 PM
 */

#include "UATokenizer.h"

#include <cstring>
#include <iostream>
#include <sstream>

using namespace std; 

UATokenizer::UATokenizer() {
}

UATokenizer::~UATokenizer() {
}

bool in_array(const char* array, char ch) {
    int i = 0;
    int len = strlen(array);
    for (; i < len; i++) {
        if (array[i] == ch) return true;
    }
    return false;
}

void UATokenizer::staticTokenize(const string &sentence, vector<string> *tokens) {
    const char sep[] = { ' ', '(', ')', '<', '>', '@', ',', ';', ':', '"', '[', ']', '?', '=', '{', '}', '\0' };

    tokens->resize(0);
    
    if (sentence.length() == 0) {    
        return;
    }
    
    char c;
    int i = 0;
    const int tokenLen = 64;
    char token[tokenLen] = {0};
    int maxi = sentence.length();
    int tmp;
    ostringstream os;

    while (i < maxi) {
        c = sentence.at(i);
        
        if (in_array(sep, c)) {
            string s = os.str();
            if (s.length() > 0) { 
                try {                    
                    tokens->push_back(s);                    
                } catch (...) {
                    cout << "exception...";
                }
                // Clear the token
                os.str(string());
            }
        } else {
            os << c;
        }

        i++;
    }

    if (strlen(token) > 0) {
        tokens->push_back(string(token));
    }
    
}
