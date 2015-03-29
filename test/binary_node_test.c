/*
 * binary_node_test.c
 *
 *  Created on: Mar 28, 2015
 *      Author: tmlbl
 */
#include <assert.h>
#include <stdio.h>

#include "test_suite.h"
#include "binary_node_test.h"

/*
 * Code under test
 */
#include "../src/binary_node.c"

int i_bnode = 0;

void test_bnode_create()
{
    struct bNode *root = NULL;
    root = bNode_create();

    assert(root != NULL);

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

    success(i_bnode);
}

int run_test_bnode()
{
    printf("testing the binary tree: ");
    test_bnode_create();
    test_bnode_add();

    return i_bnode;
}
