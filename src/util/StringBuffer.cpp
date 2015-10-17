/* 
 * File:   StringBuffer.cpp
 * Author: tomaslibal
 * 
 * Created on 17 October 2015, 21:57
 */

#include "StringBuffer.h"

#include <cstdlib>
#include <string.h>

using namespace std;

StringBuffer::StringBuffer() {
}

StringBuffer::StringBuffer(const StringBuffer& orig) {
}

StringBuffer::~StringBuffer() {
}

void StringBuffer::pushTokenToBuffer(char** p_buffer, int* p_length, char* token) {
    int offset = *p_length;
    
    if (p_buffer[offset] != NULL) {
        return;
    }
    
    p_buffer[offset] = (char*)malloc(strlen(token)+1);
    strcpy(p_buffer[offset], token);
    *p_length = *p_length+1;

    // Clear the token
    memset(token, '\0', 64);
}
