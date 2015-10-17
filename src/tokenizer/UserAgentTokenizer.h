/* 
 * File:   UserAgentTokenizer.h
 * Author: tomaslibal
 *
 * Created on 17 October 2015, 15:49
 */

#ifndef USERAGENTTOKENIZER_H
#define	USERAGENTTOKENIZER_H

#include "../model/tokenList.h"

class UserAgentTokenizer {
public:
    UserAgentTokenizer();
    UserAgentTokenizer(const UserAgentTokenizer& orig);
    virtual ~UserAgentTokenizer();
    
    static void staticTokenize(const char *sentence, tokenList *tokens);
private:

};

#endif	/* USERAGENTTOKENIZER_H */

