/*
 * binary_node.c
 *
 *  Created on: Mar 28, 2015
 *      Author: tmlbl
 */

#include "binary_node.h"

struct bNode *bNode_create()
{
    struct bNode *root = NULL;
    root = malloc(sizeof(struct bNode));
    if (root == NULL) {
        return NULL;
    }

    root->uas = NULL;
    root->classes = NULL;
    root->left = NULL;
    root->right = NULL;

    return root;
}
