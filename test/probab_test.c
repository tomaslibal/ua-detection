#include <assert.h>
#include <stdio.h>

#include "../src/probab.c"

#include "test_suite.h"

int i_probab = 0;

void test_count_words()
{
	struct uas_record *record1 = NULL;
	struct link_node_int *dict = NULL;
	struct link_node_int *iterator = NULL;

	int count = 0;

	record1 = uas_record_create();

	uas_record_set(record1, "sample_class", "Three baz and three minky and winky minky and the sun", NULL);

	dict = link_node_int_create();

	count_words(record1, dict);

	iterator = dict;
	while(iterator != NULL) {
	    count++;
		iterator = iterator->next;
	}

	assert(count == 8);

	uas_record_free(record1);
	link_node_int_free(dict);

	success(i_probab);
}

int run_test_probab()
{
    printf("testing the probab: ");

	test_count_words();

	printf("\n");
	return i_probab;
}
