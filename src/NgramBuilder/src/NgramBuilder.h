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

#define NGRAM_MAX_SIZE 32

/*
 * n-gram representation
 * n <= 16
 */
struct NgramSimple {
    int len;
    std::string sentence;
    int starts[NGRAM_MAX_SIZE];
    int lens[NGRAM_MAX_SIZE];
    std::string toString();
    std::string toString(int num);
    bool equals (NgramSimple const& other);
    unsigned int hash();
};

class NgramSimpleLengthExceededException: virtual public std::exception{
public:
    NgramSimpleLengthExceededException(std::string m="Max length of NgramSimple exceeded!") : msg(m) {}
    ~NgramSimpleLengthExceededException() throw() {}
    const char* what() const throw() { return msg.c_str(); }
private:
    std::string msg;
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
    
    //void print(NgramSimple& ng);

    int fromTokenList(char** tokens, const int numTokens, std::vector<NgramSimple> *ngrams);
    
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

