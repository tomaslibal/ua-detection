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

int run_test_bnode()
{
    printf("testing the binary tree: ");
    test_bnode_create();

    return i_bnode;
}
