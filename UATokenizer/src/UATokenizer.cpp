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

UATokenizer::UATokenizer(string uas) {
    this->uas = uas;
}

UATokenizer::UATokenizer(const UATokenizer& orig) {
    this->uas = orig->uas;
}

UATokenizer::~UATokenizer() {
}

/*
 * Returns true if character ch is in array
 */
bool in_array(const char* array, char ch) {
    int i = 0;
    int len = strlen(array);
    for (; i < len; i++) {
        if (array[i] == ch) return true;
    }
    return false;
}

/*
 * Splits a string sentence into a vector of string tokens
 */
void UATokenizer::staticTokenize(const string &sentence, vector<string> *tokens) {
    /*
     * These separators are used to determine where each token ends. They are
     * taken from the RFC-2616. I had made a decision to omit \ and / characters
     * from the list which was before the program used n-grams. Without n-grams,
     * splitting tokens by the forward slash character produced less precise
     * results because of the word independence assumption of each token. This
     * assumption is (somewhat) eliminated by using n-grams instead of a bag of 
     * words.
     */
    const char sep[] = { ' ', '(', ')', '<', '>', '@', ',', ';', ':', '"', '[', ']', '?', '=', '{', '}', '\0' };

    tokens->resize(0);
    
    if (sentence.length() == 0) {    
        return;
    }
    
    char c;
    int i = 0;
    int maxi = sentence.length();
    /*
     * This string stream grows character by character as they are read from
     * the sentence, until a separator character is encountered. Then the buffer
     * is cleared and starts anew.
     */
    ostringstream os;

    /*
     * This is a character by character consumption loop of the sentence
     */
    while (i < maxi) {
        c = sentence.at(i);
        
        /*
         * Test if the current character is a separator and if so, push the
         * buffer into the vector of tokens as a new token.
         */
        if (in_array(sep, c)) {
            string s = os.str();
            /*
             * Don't create a token if the buffer is empty. E.g. if the sentence
             * starts with a separator.
             */
            if (s.length() > 0) { 
                try {                    
                    tokens->push_back(s);                    
                } catch (...) {
                    cout << "Exception while adding a token " << s;
                }
                // Clear the token
                os.str(string());
            }
        } else {
            /*
             * The character c was not in a separator, so add it to the buffer
             * to build up a token.
             */
            os << c;
        }
        /*
         * Move by one character forward 
         */
        i++;
    }

    /*
     * Unless the sentence ended with a separator, the last buffer/token has
     * not been pushed to the vector as a new token so we do it now, but only
     * if its length is greater than 0.
     */
    if (os.str().length() > 0) {
        tokens->push_back(os.str());
    }
    
}
