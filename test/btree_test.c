/*
 * binary_node_test.c
 *
 *  Created on: Mar 28, 2015
 *      Author: tmlbl
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test_suite.h"
#include "../src/btree.c"
#include "btree_test.h"

int i_bnode = 0;

void test_bnode_create()
{
    struct bNode *root = NULL;
    root = bNode_create();

    assert(root != NULL);

    bNode_free(root);

    success(i_bnode);
}

void test_bnode_add()
{
    struct bNode *root = NULL;
    root = bNode_create();

    struct bNode *rn = NULL;
    rn = bNode_create();

    struct bNode *nested = NULL;
    nested = bNode_create();

    root->uas = malloc(sizeof(char) * 2);
    strcpy(root->uas, "m");

    rn->uas = malloc(sizeof(char) * 2);
    strcpy(rn->uas, "n");

    nested->uas = malloc(sizeof(char) * 2);
    strcpy(nested->uas, "x");

    bNode_add(rn, root);
    bNode_add(nested, root);

    assert(root->right == rn);
    assert(rn->right == nested);

    bNode_free(root);

    success(i_bnode);
}

void test_bnode_set()
{
    struct bNode *root = NULL;
    root = bNode_create();

    struct bNode *rn = NULL;
    rn = bNode_create();

    bNode_set(root, "Some UAS", 0, NULL, NULL, rn);

    assert(root->uas != NULL);
    assert(strcmp(root->uas, "Some UAS") == 0);
    assert(root->right = rn);
    assert(root->left == NULL);
    assert(root->classes == NULL);

    bNode_free(root);

    success(i_bnode);
}

void test_bnode_set_with_classes()
{
    struct bNode *root = NULL;
    root = bNode_create();

    struct link_node_int *classes = NULL;
    classes = link_node_int_create();
    link_node_int_set(classes, "some_class", 1);

    bNode_set(root, "Some UAS", 0, classes, NULL, NULL);

    assert(root->classes != NULL);
    assert(root->classes == classes);

    bNode_free(root);

    success(i_bnode);
}

void test_bnode_get()
{
    struct bNode *root = NULL;
    root = bNode_create();

    struct bNode *rn = NULL;
    rn = bNode_create();

    struct bNode *nested = NULL;
    nested = bNode_create();

    root->uas = malloc(sizeof(char) * 2);
    strcpy(root->uas, "m");

    rn->uas = malloc(sizeof(char) * 2);
    strcpy(rn->uas, "n");

    nested->uas = malloc(sizeof(char) * 2);
    strcpy(nested->uas, "x");

    bNode_add(rn, root);
    bNode_add(nested, root);

    struct bNode *result = NULL;
    result = bNode_get(root, "x");

    assert(result != NULL);

    result = bNode_get(root, "y");

    assert(result == NULL);

    bNode_free(root);

    success(i_bnode);
}

void test_bnode_remove_unique()
{
    struct bNode *root = NULL;
    root = bNode_create();

    struct bNode *rn = NULL;
    rn = bNode_create();

    struct bNode *nested = NULL;
    nested = bNode_create();

    root->uas = malloc(sizeof(char) * 2);
    strcpy(root->uas, "m");

    rn->uas = malloc(sizeof(char) * 2);
    strcpy(rn->uas, "n");

    nested->uas = malloc(sizeof(char) * 2);
    strcpy(nested->uas, "x");

    bNode_add(rn, root);
    bNode_add(nested, root);

    assert(root->right == rn);

    bNode_remove_unique(root, rn);

    assert(root->right != rn);

    struct bNode *result = NULL;

    result = bNode_get(root, "n");

    assert(result == NULL);

    result = bNode_get(root, "x");

    assert(result == nested);

    bNode_free(root);

    success(i_bnode);
}

void test_bnode_get_by_id()
{
    struct bNode *root = NULL;
    root = bNode_create();

    struct bNode *rn = NULL;
    rn = bNode_create();

    struct bNode *nested = NULL;
    nested = bNode_create();

    bNode_set(root, "some uas", 1, NULL, NULL, NULL);
    bNode_set(rn, "<another>", 2, NULL, NULL, NULL);
    bNode_set(nested, "uas-3", 3, NULL, NULL, NULL);

    bNode_add(rn, root);
    bNode_add(nested, root);

    struct bNode *result = NULL;

    result = bNode_get_by_id(root, 3);

    assert(result == nested);

    result = bNode_get_by_id(root, 2);

    assert(rn == result);

    bNode_free(root);
    success(i_bnode);
}

int run_test_bnode()
{
    printf("testing the binary tree: ");
    test_bnode_create();
    test_bnode_add();
    test_bnode_set();
    test_bnode_set_with_classes();
    test_bnode_get();
    test_bnode_remove_unique();
    test_bnode_get_by_id();

    return i_bnode;
}
