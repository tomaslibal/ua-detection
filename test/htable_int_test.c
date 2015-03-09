#include <assert.h>
#include <string.h>

#include "../src/htable_int.c"

void test_htable_create()
{
	struct htable_int *table;

	table = htable_int_create();

	assert(table != NULL);

	htable_int_free(table);
}

void test_htable_set()
{
	struct htable_int *table;
	table = htable_int_create();

	htable_int_set(table, "Some_name", 10);

	assert(strcmp(table->name, "Some_name") == 0);
	assert(table->val == 10);

	htable_int_free(table);
}

void test_htable_free_one()
{
	struct htable_int *table;

	table = htable_int_create();

	htable_int_set(table, "Some_name", 10);

	htable_int_free(table);
}

void test_htable_free_linked()
{
	struct htable_int *table, *second;

	table = htable_int_create();
	second = htable_int_create();

	table->next = second;

	htable_int_set(table, "Some_name", 10);
	htable_int_set(second, "Second_table", 20);

	htable_int_free(table);
}

void test_htable_get_one()
{
	struct htable_int *table;

	table = htable_int_create();

	htable_int_set(table, "Some_name", 10);

	assert(htable_int_get(table, "Some_name") == 10);

	htable_int_free(table);
}

void test_htable_get_linked()
{
	struct htable_int *table, *second;

	table = htable_int_create();
	second = htable_int_create();

	table->next = second;

	htable_int_set(table, "Some_name", 10);
	htable_int_set(second, "Second_table", 20);

	assert(htable_int_get(table, "Second_table") == 20);

	htable_int_free(table);
}

void test_htable_get_not_present()
{
	struct htable_int *table;

	table = htable_int_create();

	htable_int_set(table, "Some_name", 10);

	assert(htable_int_get(table, "NotPresent") == 0);

	htable_int_free(table);
}


void run_test_htable_int()
{
    test_htable_create();
    test_htable_set();
    //test_htable_free_one();
    //test_htable_free_linked();
    test_htable_get_one();
    test_htable_get_linked();
}
