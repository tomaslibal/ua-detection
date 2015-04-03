/*
 * binary_node.h
 *
 *  Created on: Mar 28, 2015
 *      Author: tmlbl
 */

#ifndef BINARY_NODE_H
#define BINARY_NODE_H

#include "link_node.h"

/*
 * Binary Node
 *
 * |----------------------|
 * | char *uas ------------------> user-agent string
 * |----------------------|
 * | struct link_node * ---------> classes
 * |----------------------|
 * | struct bNode *left ---------> left bNode
 * |----------------------|
 * | struct bNode *right --------> right bNode
 * |----------------------|
 *
 */
struct bNode {
    char *uas;
    struct link_node_int *classes;
    struct bNode *left;
    struct bNode *right;
};

struct bNode *bNode_create();

/*
 * Frees the binary free recursively starting at the passed *root node
 */
void bNode_free(struct bNode *root);

/*
 * When setting the fields of the struct, arguments which were passed NULL
 * will not override the pre-existing value of the field.
 *
 * E.g. bNode_set(someNode, NULL, NULL, left, NULL); will only update
 * someNode->left = left and will not override/re-set someNode->uas to NULL
 * and the like for the rest of the fields.
 */
void bNode_set(struct bNode *node, char *uas, struct link_node_int *classes, struct bNode *left, struct bNode *right);

/*
 * root is a node in a binary tree and the node passed into this function is
 * another binary tree node; the node will be added to the tree such that it
 * will be assigned as a left or right value of a leaf node in the tree
 * depending on the result of strcmp operation on root->uas and node->uas.
 */
void bNode_add(struct bNode *node, struct bNode *root);

/*
 * Looks up a binary node by the value of its uas field.
 */
struct bNode *bNode_get(struct bNode *node, char *uas);

/*
 * Remove operation: removes the specified node but keeps all of its ancestors
 * down the tree
 */
void bNode_remove_unique(struct bNode *root, struct bNode *node);

struct bNode *bNode_get_parent(struct bNode *root, struct bNode *node);
struct bNode *bNode_get_parent_(struct bNode *root, struct bNode *node);
void bNode_add_recursive(struct bNode *root, struct bNode *root_add);
void bNode_add_recursive_(struct bNode *root, struct bNode *node);

#endif /* SRC_BINARY_NODE_H_ */
