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

	assert(htable_int_get_val(table, "Some_name") == 10);

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

	assert(htable_int_get_val(table, "Second_table") == 20);

	htable_int_free(table);
}

void test_htable_get_not_present()
{
	struct htable_int *table;

	table = htable_int_create();

	htable_int_set(table, "Some_name", 10);

	assert(htable_int_get_val(table, "NotPresent") == 0);

	htable_int_free(table);
}

void test_htable_get_table()
{
	struct htable_int *root, *second, *result;
	root = htable_int_create();
	second = htable_int_create();

	root->next = second;

	htable_int_set(root, "First", 10);
	htable_int_set(second, "Second", 20);

	result = htable_int_get(root, "Second");

	assert(result != NULL);
	assert(result->val == 20);

	htable_int_free(root);
}

void test_htable_get_last()
{
	struct htable_int *root, *second, *last, *result;
	root = htable_int_create();
	second = htable_int_create();
	last = htable_int_create();

	root->next = second;
	second->next = last;

	htable_int_set(root, "First", 10);
	htable_int_set(second, "Second", 20);
	htable_int_set(last, "Last", 30);

	result = htable_int_get_last(root);

	assert(result != NULL);
	assert(result->val == 30);

	htable_int_free(root);
}

void test_htable_get_last_when_no_next()
{
	struct htable_int *root = NULL;
	struct htable_int *result = NULL;
	root = htable_int_create();

	htable_int_set(root, "First", 10);

	result = htable_int_get_last(root);

	assert(result != NULL);
	assert(result->val == 10);

	htable_int_free(root);
}

void test_sum_val_rec()
{
    int res = 0;

    struct htable_int *root = NULL;
    struct htable_int *second = NULL;
    struct htable_int *third = NULL;

    root = htable_int_create();
    second = htable_int_create();
    third = htable_int_create();

    htable_int_set(root, "a", 10);
    htable_int_set(root, "b", 20);
    htable_int_set(root, "c", 30);

    root->next = second;
    second->next = third;

    res = sum_val_rec(root);

    printf("sum_val_rec = %d\n", res);

    assert(res == 60);

    htable_int_free(root);
}

void run_test_htable_int()
{
    test_htable_create();
    test_htable_set();
    test_htable_get_one();
    test_htable_get_linked();
    test_htable_get_table();
    test_htable_get_last();
    test_htable_get_last_when_no_next();
    test_sum_val_rec();
}
