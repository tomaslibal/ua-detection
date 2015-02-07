/*

  Sample program implementing a tokenizer of the User-Agent header field.

  This program takes no arguments.

  August 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 a list of separators as defined in the section 2.2 of the HTTP/1.1 standard
 that has been slightly edited - removed "\", "/", HT characters
*/
char separators[] = { ' ', '(', ')', '<', '>', '@', ',', ';', ':', '"', '[', ']', '?', '=', '{', '}' };

/**
 * Checks if the char ch is in the array of characters separators.
 * @returns 1 if found 0 otherwise
 */
int in_array(char* separators, char ch)
{
    int i = 0;
    int len = strlen(separators);
    for(;i < len;i++) {
        if(separators[i] == ch) return 1;
    }
    return 0;
}

/*
 * 
 *
 *
 */
void push_tok(char** p_buffer, int* p_length, char* token)
{ 
    p_buffer[*p_length] = (char*)malloc(strlen(token)+1);
    strcpy(p_buffer[*p_length], token);
    *p_length = *p_length+1;
    
    // Clear the token
    memset(token, '\0', 64);
}

/**
 * tokenizes the user_agent string using own implementation. strtok() didn't
 * somehow work which might be my bad usage of the function or data...This
 * function reads the user_agent string char by char and builds a token until
 * it reads a separator character - then it copies the token string into p_buffer
 * array, increases *p_length by one and clears out the token string and starts
 * anew, and repeats this process until it reaches the end of the user_agent
 * string.
 *
 * @returns 0
 */
int tok(char* user_agent, char** p_buffer, int* p_length)
{
    *p_length      = 0;
    int i          = 0;
    char token[64] = {0};
    char c         = '\0';
    int len        = strlen(user_agent);
    int tmp        = 0;

    while(i<len) {
        // (user_agent + 1) moves to the i-th character of the string, esentially
        // skipping those characters that have already been visited.
        strncpy(&c, (user_agent + i), 1);

        // a separator found
        if(in_array(separators, c) == 1) {
            if(strlen(token)==0) { i++; continue; }
            push_tok(p_buffer, p_length, token);
        } else {
            // char wasn't a separator, add the character to the end of
            // the token string
            tmp = strlen(token);
            strncpy((token+tmp), &c, 1);
        }
        i++;
    }
    // If the last character wasn't a separator, there might be one more token
    if(strlen(token)>0) {
        p_buffer[*p_length] = (char*)malloc(strlen(token)+1);
        strcpy(p_buffer[*p_length], token);
        *p_length = *p_length+1;
    }

    return 0;
}

int main(int argc, char** argv)
{
    char* user_agent = "Mozilla/5.0 (Linux; en-us; Android 4.4) Firefox/28.0\0";
    char* tokens[32];
    int number;
    int* p_number = &number;

    printf("user_agent = '%s'\n", user_agent);

    if(tok(user_agent, tokens, p_number) != 0) {
        printf("Error executing 'tok'\n");
        return 1;
    }

    printf("Number of tokens = %d\n", number);

    for(int i = 0; i < number; i++) {
        printf("token %d = %s\n", i, tokens[i]);
        free(tokens[i]);
    }

    return 0;
}
