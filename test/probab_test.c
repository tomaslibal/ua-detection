#include <assert.h>

#include "../src/probab.c"

void test_count_words()
{
	struct uas_record *record1 = NULL;
	struct htable_int *dict = NULL;
	struct htable_int *iterator = NULL;

	record1 = uas_record_create();

	uas_record_set(record1, "sample_class", "Three baz and three minky and winky minky and the sun", NULL);

	dict = htable_int_create();

	count_words(record1, dict);

	iterator = dict;
	while(iterator != NULL) {
		printf("word: %s, count: %d\n", iterator->name, iterator->val);
		iterator = iterator->next;
	}

	assert(1 == 1);
}

void run_test_probab()
{
	test_count_words();
}
