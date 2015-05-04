#include <assert.h>
#include <string.h>

#include "../src/linked_list.c"
#include "test_suite.h"

/*
 * Success tests counter
 */
int i_link_node = 0;

void test_link_node_create()
{
	struct link_node_int *table;

	table = link_node_int_create();

	assert(table != NULL);

	link_node_int_free(table);

	success(i_link_node);
}

void test_link_node_set()
{
	struct link_node_int *table;
	table = link_node_int_create();

	link_node_int_set(table, "Some_name", 10);

	assert(strcmp(table->name, "Some_name") == 0);
	assert(table->val == 10);

	link_node_int_free(table);

	success(i_link_node);
}

void test_link_node_free_one()
{
	struct link_node_int *table;

	table = link_node_int_create();

	link_node_int_set(table, "Some_name", 10);

	link_node_int_free(table);

	success(i_link_node);
}

void test_link_node_free_linked()
{
	struct link_node_int *table, *second;

	table = link_node_int_create();
	second = link_node_int_create();

	table->next = second;

	link_node_int_set(table, "Some_name", 10);
	link_node_int_set(second, "Second_table", 20);

	link_node_int_free(table);

	success(i_link_node);
}

void test_link_node_get_one()
{
	struct link_node_int *table;

	table = link_node_int_create();

	link_node_int_set(table, "Some_name", 10);

	assert(link_node_int_get_val(table, "Some_name") == 10);

	link_node_int_free(table);

	success(i_link_node);
}

void test_link_node_get_linked()
{
	struct link_node_int *table, *second;

	table = link_node_int_create();
	second = link_node_int_create();

	table->next = second;

	link_node_int_set(table, "Some_name", 10);
	link_node_int_set(second, "Second_table", 20);

	assert(link_node_int_get_val(table, "Second_table") == 20);

	link_node_int_free(table);

	success(i_link_node);
}

void test_link_node_get_not_present()
{
	struct link_node_int *table;

	table = link_node_int_create();

	link_node_int_set(table, "Some_name", 10);

	assert(link_node_int_get_val(table, "NotPresent") == 0);

	link_node_int_free(table);

	success(i_link_node);
}

void test_link_node_get_table()
{
	struct link_node_int *root, *second, *result;
	root = link_node_int_create();
	second = link_node_int_create();

	root->next = second;

	link_node_int_set(root, "First", 10);
	link_node_int_set(second, "Second", 20);

	result = link_node_int_get(root, "Second");

	assert(result != NULL);
	assert(result->val == 20);

	link_node_int_free(root);

	success(i_link_node);
}

void test_link_node_get_last()
{
	struct link_node_int *root, *second, *last, *result;
	root = link_node_int_create();
	second = link_node_int_create();
	last = link_node_int_create();

	root->next = second;
	second->next = last;

	link_node_int_set(root, "First", 10);
	link_node_int_set(second, "Second", 20);
	link_node_int_set(last, "Last", 30);

	result = link_node_int_get_last(root);

	assert(result != NULL);
	assert(result->val == 30);

	link_node_int_free(root);

	success(i_link_node);
}

void test_link_node_get_last_when_no_next()
{
	struct link_node_int *root = NULL;
	struct link_node_int *result = NULL;
	root = link_node_int_create();

	link_node_int_set(root, "First", 10);

	result = link_node_int_get_last(root);

	assert(result != NULL);
	assert(result->val == 10);

	link_node_int_free(root);

	success(i_link_node);
}

void test_sum_val_rec()
{
    int res = 0;

    struct link_node_int *root = NULL;
    struct link_node_int *second = NULL;
    struct link_node_int *third = NULL;

    root = link_node_int_create();
    second = link_node_int_create();
    third = link_node_int_create();

    link_node_int_set(root, "a", 10);
    link_node_int_set(second, "b", 20);
    link_node_int_set(third, "c", 30);

    root->next = second;
    second->next = third;

    res = link_node_int_sum_val_rec(root);

    assert(res == 60);

    link_node_int_free(root);

    success(i_link_node);
}

int run_test_link_node()
{
    printf("testing the linked nodes list: ");

    test_link_node_create();
    test_link_node_set();
    test_link_node_get_one();
    test_link_node_get_linked();
    test_link_node_get_table();
    test_link_node_get_last();
    test_link_node_get_last_when_no_next();
    test_sum_val_rec();

    printf("\n");
    return i_link_node;
}
