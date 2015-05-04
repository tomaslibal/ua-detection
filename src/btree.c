/*
 * binary_node.c
 *
 *  Created on: Mar 28, 2015
 *      Author: tmlbl
 */

#include "util/btree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bNode *bNode_create()
{
    struct bNode *root = NULL;
    root = malloc(sizeof(struct bNode));
    if (root == NULL) {
        return NULL;
    }

    root->id = 0;
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

void bNode_set(struct bNode *node, char *uas, int id, struct link_node_int *classes, struct bNode *left, struct bNode *right)
{
    if (node == NULL) {
        return;
    }

    node->id = id;

    // assign uas to root->uas
    if (uas != NULL) {
        if (node->uas != NULL) {
            free(node->uas);
        }
        node->uas = malloc(sizeof(char) * strlen(uas) + 1);
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

struct bNode *bNode_get(struct bNode *root, char *uas)
{
    struct bNode *rl = NULL;
    struct bNode *rr = NULL;

    if (root == NULL) {
        return NULL;
    }

    if (root->uas != NULL & strcmp(root->uas, uas) == 0) {
        return root;
    } else {
        rl = bNode_get(root->left, uas);
        rr = bNode_get(root->right, uas);
        if (rl != NULL) return rl;
        if (rr != NULL) return rr;
    }

    return NULL;
}

void bNode_remove_unique(struct bNode *root, struct bNode *node)
{
    struct bNode *removed_root = node;
    struct bNode *parent = NULL;

    parent = bNode_get_parent(root, node);

    if (parent == NULL) {
        printf("parent not found, delete cancelled!\n");
        exit(EXIT_FAILURE);
    }

    if (parent->left == node)
        parent->left = NULL;
    else
        parent->right = NULL;

    // now, add all children of the node back to the root
    bNode_add_recursive(root, removed_root);

    // free the removed node
    removed_root->left = NULL;
    removed_root->right = NULL;
    bNode_free(removed_root);

    return;
}

// untested
struct bNode *bNode_get_parent(struct bNode *root, struct bNode *node)
{
    if (root == node) {
        return node;
    }

    return bNode_get_parent_(root, node);
}

// untested
struct bNode *bNode_get_parent_(struct bNode *root, struct bNode *node)
{
    struct bNode *left = NULL;
    struct bNode *right = NULL;

    // base case - not found
    if (root == NULL) {
        return NULL;
    }

    // base case - found
    if (root->left == node || root->right == node) {
        return root;
    }

    left = bNode_get_parent_(root->left, node);
    right = bNode_get_parent_(root->right, node);

    if (left != NULL) return left;
    if (right != NULL) return right;

    return NULL;
}

// untested
void bNode_add_recursive(struct bNode *root, struct bNode *root_add)
{
    bNode_add_recursive_(root, root_add->left);
    bNode_add_recursive_(root, root_add->right);
}

// untested
void bNode_add_recursive_(struct bNode *root, struct bNode *node)
{
    // base case
    if (node == NULL) {
        return;
    }

    // add itself
    bNode_add(node , root);
    bNode_add_recursive_(root, node->left);
    bNode_add_recursive_(root, node->right);
}

struct bNode *bNode_get_by_id(struct bNode *root, int id)
{
    struct bNode *rl = NULL;
    struct bNode *rr = NULL;

    if (root == NULL) {
        return NULL;
    }

    if (root->id == id) {
        return root;
    } else {
        rl = bNode_get_by_id(root->left, id);
        rr = bNode_get_by_id(root->right, id);
        if (rl != NULL) return rl;
        if (rr != NULL) return rr;
    }

    return NULL;
}
