/* 
 * File:   UATokenizer.cpp
 * Author: tlibal
 * 
 * Created on November 22, 2015, 7:08 PM
 */

#include "UATokenizer.h"

#include <cstring>
#include <cmath>
#include <iostream>
#include <sstream>

using std::string;
using std::vector;

UATokenizer::UATokenizer() {
}

UATokenizer::UATokenizer(const string& uas) {
    this->uas = uas;
}

UATokenizer::UATokenizer(const UATokenizer& orig) {
    this->uas = orig.uas;
}

UATokenizer::~UATokenizer() {
}

void UATokenizer::set_uas(const string& sentence)
{
    uas = sentence;
}

string UATokenizer::get_uas()
{
    return uas;
}



/*
 * Returns true if character ch is in array
 */
bool UATokenizer::in_array(const char* array, char ch) {
    int i = 0;
    int len = strlen(array);
    for (; i < len; i++) {
        if (array[i] == ch) return true;
    }
    return false;
}

/*
 * From my gist https://gist.github.com/tomaslibal/5124095
 */
bool UATokenizer::in_arrayb(const char* array, char ch)
{
    int a = 0;                                                // start of the search array
    int b = strlen(array);                                     // end of the search array
    int m = 0;                                                // the middle value
    int prev;                                                 // remembers the previous middle value
    while(true) {
        prev = m;                                             // prev gets the old middle value
        m = floor((a+b)/2);                                   // m gets a new middle value
        if(prev == m) { break; }                              // reached the end, break out of the loop
        if(array[m] == ch) { return true; }                  // found the lookup, return the index position
        // On the next line, for processing.js the comparison method will be "if(stack[m] > lookup)". The following is for processing in java:
        else if(array[m] > ch) {             // the lookup cannot be in the right hand side part of the array
            b = m - 1;                                        // so bound the end of the array at m: <a, m>
        }else {                                               // otherwise the lookup cannot be in the left hand side part of the array
            a = m + 1;                                        // so bound the search array from the left at m: <m, b>
        }
    }
    return false; // not found
}

bool UATokenizer::is_separator(char ch)
{
    if (ch == 32) { // blank space
        return true;
    }
    if (ch == 34) { // double quote
        return true;
    }
    if (ch < 32 || ch > 125) {
        return false;
    }
    if (ch == 40 || // (
        ch == 41 || // )
        ch == 44 || // ,
        ch == 58 || // :
        ch == 59 || // ;
        ch == 60 || // <
        ch == 61 || // =
        ch == 62 || // >
        ch == 63 || // ?
        ch == 64 || // @
        ch == 91 || // [
        ch == 93 || // ]
        ch == 123 || // {
        ch == 125 // }
    ) {
        return true;
    }
    
    return false;
}



/*
 * Splits a string sentence into a vector of string tokens
 */
void UATokenizer::tokenize(const string &sentence, vector<string> *tokens) {
    /*
     * These separators are used to determine where each token ends. They are
     * taken from the RFC-2616. I had made a decision to omit \ and / characters
     * from the list which was before the program used n-grams. Without n-grams,
     * splitting tokens by the forward slash character produced less precise
     * results because of the word independence assumption of each token. This
     * assumption is (somewhat) eliminated by using n-grams instead of a bag of 
     * words.
     */
    const char sep[] = { ' ', '"', '(', ')', ',', ':', ';', '<', '=', '>', '?', '@', '[', ']', '{', '}', '\0' };

    tokens->clear();
    
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
    std::ostringstream os;

    /*
     * This is a character by character consumption loop of the sentence
     */
    while (i < maxi) {
        c = sentence.at(i);
        
        /*
         * Test if the current character is a separator and if so, push the
         * buffer into the vector of tokens as a new token.
         */
        if (is_separator(c)) {
            string s = os.str();
            /*
             * Don't create a token if the buffer is empty. E.g. if the sentence
             * starts with a separator.
             */
            if (s.length() > 0) { 
                try {                    
                    tokens->push_back(s);                    
                } catch (...) {
                    std::cout << "Exception while adding a token " << s;
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

void UATokenizer::tokenize(vector<string>* tokens) {
    if (uas.empty()) {
        return;
    }
    
    tokenize(uas, tokens);
}
