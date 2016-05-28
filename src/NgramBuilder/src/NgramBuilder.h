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

/*
 * Ngram of a variable N length:
 * 
 * Suppose a 3-gram: Lazy, Lazy Fox, Lazy Fox Jumps. This data structure would capture it as follows:
 * 
 * Ngram n ...
 * n.len == 3
 * n.tokens[0] == "Lazy"
 * n.tokens[1] == "Lazy Fox"
 * n.tokens[2] == "Lazy Fox Jumps"
 * n.tokens[3-127] undefined
 * 
 */
struct Ngram {
    int len;
    std::string tokens[128];
    std::string toString();
    std::string toString(int num);
    bool equals(const Ngram& other);
};

/*
 * NgramBuilder takes a string and produces a list of Ngrams.
 * 
 */
class NgramBuilder {
public:
    NgramBuilder();
    NgramBuilder(const NgramBuilder &o);
    virtual ~NgramBuilder();
    
    int fromTokenList(std::vector<std::string> &tokens, std::vector<Ngram> *ngrams);
    void print(Ngram& ng);
    
    void set_level(const int level);
    int get_level();
private:
    int level = 3;
};

#endif	/* NGRAMBUILDER_H */

