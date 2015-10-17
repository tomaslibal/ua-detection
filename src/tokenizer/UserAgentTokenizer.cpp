/* 
 * File:   UserAgentTokenizer.cpp
 * Author: tomaslibal
 * 
 * Created on 17 October 2015, 15:49
 */

#include "UserAgentTokenizer.h"
#include "../util/StringBuffer.h"
#include "../util/utils.h"

#include <cstdlib>
#include <string.h>

using namespace std;

UserAgentTokenizer::UserAgentTokenizer() {
}

UserAgentTokenizer::UserAgentTokenizer(const UserAgentTokenizer& orig) {
}

UserAgentTokenizer::~UserAgentTokenizer() {
}

void UserAgentTokenizer::staticTokenize(const char* sentence, tokenList *tokens) {
    char sep[] = { ' ', '(', ')', '<', '>', '@', ',', ';', ':', '"', '[', ']', '?', '=', '{', '}' };

    if (sentence == NULL) {
        tokens->length = 0;
        return;
    }

    char c;
    int i = 0;
    char token[64] = {0};
    int maxi = strlen(sentence);
    int tmp;

    while(i < maxi) {
        strncpy(&c, (sentence + i), 1);
        
        if(Utils::in_array(sep, c)) {
            if(strlen(token) > 0) { 
                StringBuffer::pushTokenToBuffer(tokens->tokens, &tokens->length, token);
            }
        } else {
            tmp = strlen(token);
            strncpy((token+tmp), &c, 1);
        }

        i++;
    }

    if (strlen(token) > 0) {
        StringBuffer::pushTokenToBuffer(tokens->tokens, &tokens->length, token);
    }
}