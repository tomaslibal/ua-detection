#include <assert.h>
#include <stdio.h>


#include "dictionary_test.h"

#include "test_suite.h"

#include "../src/dictionary.c"

int i_dict = 0;

void test_dictionary_create()
{
    struct dict_link_node_int *dict = NULL;
    dict = dict_link_node_int_create();

    assert(dict != NULL);

    dict_link_node_int_free(dict);

    success(i_dict);
}

void test_dictionary_set()
{
    struct dict_link_node_int *dict = NULL;
    dict = dict_link_node_int_create();

    if (dict == NULL) {
        assert(0);
    }

    struct link_node_int *root = NULL;
    root = link_node_int_create();

    dict_link_node_int_set(dict, "SomeClass", root, NULL);

    assert(dict->class_name != NULL);
    assert(strcmp(dict->class_name, "SomeClass") == 0);
    assert(dict->root = root);

    dict_link_node_int_free(dict);

    success(i_dict);
}

void test_dictionary_find()
{
    struct dict_link_node_int *dict = NULL;
    struct dict_link_node_int *next = NULL;
    dict = dict_link_node_int_create();
    next = dict_link_node_int_create();

    if (dict == NULL) {
        assert(0);
    }

    struct link_node_int *root = NULL;
    root = link_node_int_create();

    dict_link_node_int_set(dict, "SomeClass", root, next);
    dict_link_node_int_set(next, "FindMe", NULL, NULL);

    assert(dict->next == next);

    struct dict_link_node_int *lookup = NULL;

    lookup = dict_link_node_int_find(dict, "FindMe");

    assert(lookup == next);

    dict_link_node_int_free(dict);

    success(i_dict);
}

int run_test_dictionary()
{
    printf("testing the dictionary: ");
    test_dictionary_create();
    test_dictionary_set();
    test_dictionary_find();

    printf("\n");
    return i_dict;
}
