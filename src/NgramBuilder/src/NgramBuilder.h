/* 
 * File:   NgramBuilder.h
 * Author: tlibal
 *
 * Created on November 22, 2015, 7:09 PM
 */

#ifndef NGRAMBUILDER_H
#define	NGRAMBUILDER_H

#include <vector>
#include <string>

struct Ngram {
    int len;
    std::string tokens[128];
    std::string* toString();
    std::string* toString(int num);
};

class NgramBuilder {
public:
    NgramBuilder();
    NgramBuilder(const NgramBuilder &o);
    virtual ~NgramBuilder();
    
    int level = 3;
    
    int fromTokenList(std::vector<std::string> &tokens, std::vector<Ngram> *ngrams);
    int fromUserAgentString(std::string &sentence, std::vector<Ngram> *ngrams);
    void print(Ngram& ng);
private:

};

#endif	/* NGRAMBUILDER_H */

