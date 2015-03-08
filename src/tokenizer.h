#ifndef TOKENIZER
#define TOKENIZER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int in_array(char* separators, char ch);
void push_tok(char** p_buffer, int* p_length, char* token);
int tok(char* user_agent, char** p_buffer, int* p_length);

#endif
