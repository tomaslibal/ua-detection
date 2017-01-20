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
    UATokenizer();
    UATokenizer(const std::string& uas);
    UATokenizer(const UATokenizer &orig);
    
    virtual ~UATokenizer();
    
    std::string uas = "";
    void set_uas(const std::string &sentence);
    std::string get_uas();
    
    void tokenize(const std::string &sentence, std::vector<std::string> *tokens);
    void tokenize(const std::string &sentence, char** tokens, int * num);
    void tokenize(std::vector<std::string>* tokens);
private:
    enum class States { Building_token, Close_token };
    char mem[2048];
    char token[128];

};

#endif	/* UATOKENIZER_H */

