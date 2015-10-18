/* 
 * File:   NgramBuilder.h
 * Author: tomaslibal
 *
 * Created on 17 October 2015, 15:25
 */

#ifndef NGRAMBUILDER_H
#define	NGRAMBUILDER_H

#include "../model/tokenList.h"

typedef struct Ngram {
    int len;
    char* tokens[64];
} Ngram;

class NgramBuilder {
public:
    NgramBuilder();
    NgramBuilder(const NgramBuilder& orig);
    virtual ~NgramBuilder();
    
    int level;
    
    int fromTokenList(tokenList *tokens, Ngram * &ngrams);
    int fromUserAgentString(const char *sentence, Ngram * &ngrams);
    void print(Ngram *ng);
    static void freeNgram(Ngram *ng);
private:
    
};

#endif	/* NGRAMBUILDER_H */

