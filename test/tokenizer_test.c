#include <assert.h>

#include "../src/tokenizer.c"

#define true 1
#define false 0

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
		printf("token %d = %s\n", i, tokens[i]);
	    free(tokens[i]);
	}
}

void run_test_tokenizer()
{
	test_tokenizing();
}
