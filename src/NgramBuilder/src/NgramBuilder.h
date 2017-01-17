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
    unsigned int hash();
};

struct NgramSimple {
    int len;
    std::string sentence;
    int starts[16];
    int lens[16];
    std::string toString();
    std::string toString(int num);
    bool equals (NgramSimple const& other);
    unsigned int hash();
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
    
    int fromTokenList(std::vector<std::string> const& tokens, std::vector<NgramSimple> *ngrams);
    
    void set_level(const int level);
    int get_level();
    void set_dynamic(const bool is_dynamic);
    bool is_dynamic();
private:
    int level = 3;
    /*
     * If dynamic set to true, when building ngrams from the tokens the ngrams will have
     * length [tokens.size(),...,1] progressively.
     * 
     * So if the tokens.size() = 5, the first ngram will be a 5-gram, the next one will be
     * a 4-gram, and so on down to the 1-gram for the last token in the tokens vector.
     */
    bool dynamic = false;
};

#endif	/* NGRAMBUILDER_H */

