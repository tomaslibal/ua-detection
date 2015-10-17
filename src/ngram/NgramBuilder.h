/* 
 * File:   NgramBuilder.h
 * Author: tomaslibal
 *
 * Created on 17 October 2015, 15:25
 */

#ifndef NGRAMBUILDER_H
#define	NGRAMBUILDER_H

typedef struct Ngram {
    int len;
    char* tokens[64];
} Ngram;

class NgramBuilder {
public:
    NgramBuilder();
    NgramBuilder(const NgramBuilder& orig);
    virtual ~NgramBuilder();
    
    uint level;
    
    void fromTokenList(char **tokens, uint numTokens, Ngram * &ngrams);
    void fromString(const char *sentence, Ngram * &ngrams);
    void tokenize(const char *sentence, char **out, uint *len);
    void print(Ngram *ng);
private:
    bool in_array(char *array, char ch);
    void push_tok(char **p_buffer, uint *p_length, char *token);
};

#endif	/* NGRAMBUILDER_H */

