#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "uas.h"

/*
creates an array of strings
pre-condition: tokens is an empty array, uas is a string
post-condition: tokens is an array of strings
| index | char*       |
|-------|-------------|
|     0 | "Keyword\0" |
|     1 | "Keyword\0" |
|   ... |         ... |
|     n | "Keyword\0" |
-----------------------

See https://github.com/tomaslibal/ua-detection/blob/master/doc/user_agent.md for
how the tokens are created.

returns n
*/
int tokenize(char*** tokens, char* uas)
{
    // position in the string 0-256
    uint8_t i = 0;
    // buffer for a keyword
    char b[100];
    memset(&b, 0, 100);
    // started a keyword (1=yes,0=no)
    uint8_t st = 0;
    uint8_t pos = 0;
    uint8_t max = strlen(uas);
    uint8_t cnt = 0;

    for(;i<max;i++){
        // A:65 Z:90 a:97 z:122 0:48 9:57 " ":32 \n:10 \t:9
        if((uas[i]>47&&uas[i]<58)
        ||(uas[i]>64&&uas[i]<91)
        ||(uas[i]>94&&uas[i]<123)
        ||(uas[i]>44&&uas[i]<48)) {
            b[pos++] = uas[i];
        } else if(uas[i]==32||uas[i]==10||uas[i]==9) {
            *tokens = realloc(*tokens, sizeof(**tokens)*(cnt+1));
            (*tokens)[cnt] = malloc(pos+1);
            strcpy((*tokens)[cnt++], b);
            pos = 0;
            memset(&b, 0, 100);
        }
    }
    // One last keyword if the uas didn't end with a whitespace
    if(strlen(b)>0) {
        *tokens = realloc(*tokens, sizeof(**tokens)*(cnt+1));
        (*tokens)[cnt] = malloc(strlen(b)+1);
        strcpy((*tokens)[cnt], b);
    }

    return cnt;
}
