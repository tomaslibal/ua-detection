/* 
 * File:   NgramBuilder.cpp
 * Author: tomaslibal
 * 
 * Created on 17 October 2015, 15:25
 */

#include "NgramBuilder.h"

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

bool NgramBuilder::in_array(char* array, char ch) {
    int i = 0;
    int len = strlen(array);
    for(;i < len;i++) {
        if(array[i] == ch) return true;
    }
    return false;
}

void NgramBuilder::push_tok(char** p_buffer, int* p_length, char* token) { 
    p_buffer[*p_length] = (char*)malloc(strlen(token)+1);
    strcpy(p_buffer[*p_length], token);
    *p_length = *p_length+1;

    // Clear the token
    memset(token, '\0', 64);
}

// Temp. method. as this reimplements the origin C tokenize function
void NgramBuilder::tokenize(const char *sentence, char **out, int *len) {
    char sep[] = { ' ', '(', ')', '<', '>', '@', ',', ';', ':', '"', '[', ']', '?', '=', '{', '}' };

    if (sentence == NULL) {
        *len = 0;
        return;
    }

    char c;
    int i = 0;
    char token[64] = {0};
    int maxi = strlen(sentence);
    int tmp;

    while(i < maxi) {
        strncpy(&c, (sentence + i), 1);

        if(this->in_array(sep, c)) {
            if(strlen(token) > 0) { 
                this->push_tok(out, len, token);
            }
        } else {
            tmp = strlen(token);
            strncpy((token+tmp), &c, 1);
        }

        i++;
    }

    if (strlen(token) > 0) {
        this->push_tok(out, len, token);
    }

}

void NgramBuilder::print(Ngram *ng) {
    int l = ng->len;
    for(int i = 0; i < l; i++) {
        std::cout << ng->tokens[i] << ", ";
    }
    std::cout << endl;
}

void NgramBuilder::fromTokenList(char **tokens, int numTokens, Ngram*& ngrams) {
    ngrams = (Ngram *)realloc(ngrams, sizeof(Ngram) * (numTokens));

    int slider = 0;
    int u = 0;

    for(; slider < numTokens; slider++) {
        // num remaining tokens must be > this->level
        int rem = numTokens - (slider+this->level);
        if (rem < 0) {
            break;
        }

        ngrams[slider].len = this->level;

        for(int j = 0; j < this->level; j++) {
            ngrams[slider].tokens[j] = (char *)malloc(strlen(tokens[slider+j]));
            strcpy(ngrams[slider].tokens[j], tokens[slider+j]);

        }
    }
}

void NgramBuilder::fromString(const char *sentence, Ngram * &ngrams) {
    char* tokens[64];
    
    int length = 0;
    int* len = &length;
    this->tokenize(sentence, tokens, len);

    this->fromTokenList(tokens, length, ngrams);
}
