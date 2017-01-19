/* 
 * File:   UATokenizer.cpp
 * Author: tlibal
 * 
 * Created on November 22, 2015, 7:08 PM
 */

#include "UATokenizer.h"

#include <cstring>

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
    //const char sep[] = { ' ', '"', '(', ')', ',', ':', ';', '<', '=', '>', '?', '@', '[', ']', '{', '}', '\0' };

    tokens->clear();
    
    if (sentence.length() == 0) {
        return;
    }
    
    char * chr = new char [sentence.length()+1];
    std::strcpy (chr, sentence.c_str());
    
    char * tok = new char [128]();
    short int i = 0;
    
    States state;
    
    /* 
     * set the starting state
     */
    if (*chr != ' ') {
        state = States::Building_token;
    } else {
        state = States::Close_token;
    }
    
    /*
     * consume the character array by sequences of bytes where we either:
     * 
     *  - append to the token
     *  - finish the token creation and push it to the stack
     */
    while (*chr != '\0') {
        switch (state) {
            case States::Building_token:
                do {
                    tok[i] = *chr;
                    i++;
                    
                    if (*chr++ == ' ') {
                        state = States::Close_token;
                        break;
                    }
                } while (*chr != '\0');
                break;

            case States::Close_token:
                if (i > 0) {
                    tok[i-1] = 0; // fix this issue with the extra whitespace
                    tokens->push_back(tok);
                    memset(tok, 0, 128);
                    i = 0;
                }
                
                do {
                    if (*chr != ' ') {
                        state = States::Building_token;
                        break;
                    }
                } while(*chr++ == ' ');
                state = States::Building_token;
                break;
        }
    }

    /*
     * Unless the sentence ended with a separator, the last buffer/token has
     * not been pushed to the vector as a new token so we do it now, but only
     * if its length is greater than 0.
     */
    if (i > 0) {
      tokens->push_back(tok);
    }
    
    //delete[] chr;
    //delete[] tok;
    
}

void UATokenizer::tokenize(vector<string>* tokens) {
    if (uas.empty()) {
        return;
    }
    
    tokenize(uas, tokens);
}
