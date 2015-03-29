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
    struct link_node *classes;
    struct bNode *left;
    struct bNode *right;
};

struct bNode *bNode_create();

/*
 * Frees the binary free recursively starting at the passed *root node
 */
void bNode_free(struct bNode *root);

/*
 * root is a node in a binary tree and the node passed into this function is
 * another binary tree node; the node will be added to the tree such that it
 * will be assigned as a left or right value of a leaf node in the tree
 * depending on the result of strcmp operation on root->uas and node->uas
 */
void bNode_add(struct bNode *node, struct bNode *root);

#endif /* SRC_BINARY_NODE_H_ */
