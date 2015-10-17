/* 
 * File:   StringBuffer.h
 * Author: tomaslibal
 *
 * Created on 17 October 2015, 21:57
 */

#ifndef STRINGBUFFER_H
#define	STRINGBUFFER_H

class StringBuffer {
public:
    StringBuffer();
    StringBuffer(const StringBuffer& orig);
    virtual ~StringBuffer();
    
    static void pushTokenToBuffer(char** p_buffer, int* p_length, char* token);
private:

};

#endif	/* STRINGBUFFER_H */

