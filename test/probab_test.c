#include <assert.h>
#include <stdio.h>

#include "../src/probab.c"

#include "test_suite.h"

int i_probab = 0;

void test_count_words()
{
	struct uas_record *record1 = NULL;
	struct htable_int *dict = NULL;
	struct htable_int *iterator = NULL;

	int count = 0;

	record1 = uas_record_create();

	uas_record_set(record1, "sample_class", "Three baz and three minky and winky minky and the sun", NULL);

	dict = htable_int_create();

	count_words(record1, dict);

	iterator = dict;
	while(iterator != NULL) {
	    count++;
		iterator = iterator->next;
	}

	assert(count == 8);

	success(i_probab);
}

int run_test_probab()
{
    printf("testing the probab: ");

	test_count_words();

	printf("\n");
	return i_probab;
}
