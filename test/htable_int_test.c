#include <assert.h>

#include "../src/htable_int.c"

void test_htable_create()
{
	struct htable_int *table;

	table = htable_int_create();

	assert(table != NULL);
}

void run_test_htable_int()
{
    test_htable_create();
}
