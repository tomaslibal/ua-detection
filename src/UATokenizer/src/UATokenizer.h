/* 
 * File:   UATokenizer.h
 * Author: tlibal
 *
 * Created on November 22, 2015, 7:08 PM
 */

#ifndef UATOKENIZER_H
#define	UATOKENIZER_H

#include <cstdlib>
#include <string>
#include <vector>

#include "../../NgramBuilder/src/NgramBuilder.h"

/*
 * User-agent Tokenizer. Given a user-agent string, this class can segment the
 * string into tokens, as specified by the RFC2616.
 */
class UATokenizer {
public:
    UATokenizer();
    UATokenizer(const std::string& uas);
    UATokenizer(const UATokenizer &orig);
    
    virtual ~UATokenizer();
    
    void tokenize(const std::string &sentence, char** tokens, int * num);
private:
    enum class States { Building_token, Close_token };

};

#endif	/* UATOKENIZER_H */

