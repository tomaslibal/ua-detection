/* 
 * File:   NgramBuilder.cpp
 * Author: tlibal
 * 
 * Created on November 22, 2015, 7:09 PM
 */

#include <cassert>

#include "NgramBuilder.h"
#include "../../UATokenizer/src/UATokenizer.h"

using namespace std;

string* Ngram::toString() {
    string* str = new string();
    for(int i = 0; i < len; i++) {
        
        str->append(tokens[i]);
        
        if (i < (len-1)) {
            str->append(" ");
        }
    }
    return str;
}

string* Ngram::toString(int num) {
    
    assert(len >= num);
    
    string* str = new string();
    for(int i = 0; i < num; i++) {
        
        str->append(tokens[i]);
        
        if (i < (len-1)) {
            str->append(" ");
        }
    }
    return str;
}

NgramBuilder::NgramBuilder() {
}

NgramBuilder::NgramBuilder(const NgramBuilder& orig) {
    level = orig.level;
}

NgramBuilder::~NgramBuilder() {
}

int NgramBuilder::fromTokenList(vector<std::string> &tokens, vector<Ngram> *ngrams) {
    int size = tokens.size();
    int slider = 0;
    int u = 0;

    for(; slider < size; slider++) {
        // num remaining tokens must be > this->level
        int rem = size - (slider+this->level);
        if (rem < 0) {
            break;
        }

        Ngram n;
        n.len = level;

        for(int j = 0; j < this->level; j++) {
            n.tokens[j] = tokens.at(slider + j);
        }
        
        ngrams->push_back(n);
    }
    
    return slider;
}

int NgramBuilder::fromUserAgentString(string &sentence, vector<Ngram> *ngrams) {    
    UATokenizer tok;
    vector<string> uaTokens;
    
    tok.staticTokenize(sentence, &uaTokens);
    
    return this->fromTokenList(uaTokens, ngrams);
}

