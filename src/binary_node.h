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

void bNode_create();

#endif /* SRC_BINARY_NODE_H_ */
