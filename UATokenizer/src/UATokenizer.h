/* 
 * File:   UATokenizer.h
 * Author: tlibal
 *
 * Created on November 22, 2015, 7:08 PM
 */

#ifndef UATOKENIZER_H
#define	UATOKENIZER_H

#include <string>
#include <vector>

#include "../../NgramBuilder/src/NgramBuilder.h"

/*
 * User-agent Tokenizer. Given a user-agent string, this class can segment the
 * string into tokens, as specified by the RFC2616.
 */
class UATokenizer {
public:
    UATokenizer(std::string uas);
    UATokenizer(const UATokenizer &orig);
    
    virtual ~UATokenizer();
    
    std::string uas;
    
    static void staticTokenize(const std::string &sentence, std::vector<std::string> *tokens);
private:
    

};

#endif	/* UATOKENIZER_H */

