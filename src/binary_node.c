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


void bNode_add(struct bNode *node, struct bNode *root)
{
    //if (root == NULL)

    int d = strcmp(root->uas, node->uas);

    if (d <= 0) {
        if (root->right == NULL) {
            root->right = node;
        } else {
            bNode_add(node, root->right);
        }
    } else {
        if (root->left == NULL) {
            root->left = node;
        } else {
            bNode_add(node, root->left);
        }
    }
}
