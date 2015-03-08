#include <assert.h>
#include <string.h>

#include "../src/htable_int.c"

void test_htable_create()
{
	struct htable_int *table;

	table = htable_int_create();

	assert(table != NULL);
}

void test_htable_set()
{
	struct htable_int *table;
	table = htable_int_create();

	htable_int_set(table, "Some_name", 10);

	assert(strcmp(table->name, "Some_name") == 0);
	assert(table->val == 10);
}

void run_test_htable_int()
{
    test_htable_create();
    test_htable_set();
}
