/*
 * binary_node.c
 *
 *  Created on: Mar 28, 2015
 *      Author: tmlbl
 */

#include "btree.h"

#include <stdlib.h>
#include <string.h>

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

void bNode_free(struct bNode *root)
{
    if (root == NULL) {
        return;
    }

    struct bNode *tmp = NULL;
    struct bNode *tmp_prev = NULL;

    tmp = root;

    /*
     * uas is a char * pointer
     */
    if (root->uas != NULL) {
        free(root->uas);
    }

    /*
     * classes is a link_node_int pointer
     */
    if (root->classes != NULL) {
        link_node_int_free(root->classes);
    }

    bNode_free(root->left);
    bNode_free(root->right);

    free(root);
}

void bNode_set(struct bNode *node, char *uas, struct link_node_int *classes, struct bNode *left, struct bNode *right)
{
    if (node == NULL) {
        return;
    }

    // assign uas to root->uas
    if (uas != NULL) {
        if (node->uas != NULL) {
            free(node->uas);
        }
        node->uas = malloc((sizeof(char) * strlen(uas)) + 1);
        if (node->uas == NULL) {
            // malloc error
            return;
        }
        strcpy(node->uas, uas);
    }

    // assign classes to root->classes
    if (classes != NULL) {
        if (node->classes != NULL) {
            link_node_int_free(node->classes);
        }
        node->classes = classes;
    }

    // assign the left link
    if (left != NULL) {
        if (node->left != NULL) {
            bNode_free(node->left);
        }
        node->left = left;
    }

    // assign the right link
    if (right != NULL) {
        if (node->right != NULL) {
            bNode_free(node->right);
        }
        node->right = right;
    }
}
