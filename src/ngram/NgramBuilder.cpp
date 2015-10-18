/* 
 * File:   NgramBuilder.cpp
 * Author: tomaslibal
 * 
 * Created on 17 October 2015, 15:25
 */

#include "NgramBuilder.h"
#include "../util/StringBuffer.h"
#include "../tokenizer/UserAgentTokenizer.h"

#include <string.h>
#include <cstdlib>
#include <iostream>

using namespace std;

NgramBuilder::NgramBuilder() {
    this->level = 3;
}

NgramBuilder::NgramBuilder(const NgramBuilder& orig) {
}

NgramBuilder::~NgramBuilder() {
}

void NgramBuilder::print(Ngram *ng) {
    int l = ng->len;
    for(int i = 0; i < l; i++) {
        std::cout << ng->tokens[i] << ", ";
    }
    std::cout << endl;
}

int NgramBuilder::fromTokenList(tokenList *tokens, Ngram*& ngrams) {
    ngrams = (Ngram *)realloc(ngrams, sizeof(Ngram) * (tokens->length));

    int slider = 0;
    int u = 0;

    for(; slider < tokens->length; slider++) {
        // num remaining tokens must be > this->level
        int rem = tokens->length - (slider+this->level);
        if (rem < 0) {
            break;
        }

        ngrams[slider].len = this->level;

        for(int j = 0; j < this->level; j++) {
            ngrams[slider].tokens[j] = (char *)malloc(strlen(tokens->tokens[slider+j]));
            strcpy(ngrams[slider].tokens[j], tokens->tokens[slider+j]);
        }
    }
    
    return slider;
}

int NgramBuilder::fromUserAgentString(const char *sentence, Ngram * &ngrams) {
    tokenList *tokens = (tokenList*)malloc(sizeof(tokenList));
    
    UserAgentTokenizer::staticTokenize(sentence, tokens);
    
    return this->fromTokenList(tokens, ngrams);
}
