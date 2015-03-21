#include <assert.h>

#include "../src/tokenizer.c"

#include "test_suite.h"

#define true 1
#define false 0

int i_tok = 0;

void test_tokenizing()
{
	char *sample = "Mozilla/5.0 (Linux; Cool stuff) like Gecko Firefox/28.0";
	char* tokens[12];
	int number;
	int* p_number = &number;

	if(tok(sample, tokens, p_number) != 0) {
		printf("Error executing 'tok'\n");
	}

	assert(*p_number == 7);

	assert(strcmp(tokens[0], "Mozilla/5.0") == 0);
	assert(strcmp(tokens[3], "stuff") == 0);

	for(int i = 0; i < number; i++) {
	    free(tokens[i]);
	}

	success(i_tok);
}

int run_test_tokenizer()
{
    printf("testing the tokenizer: ");

	test_tokenizing();

	printf("\n");
	return i_tok;
}
