/*
 * db.h
 *
 *  Created on: Apr 4, 2015
 *      Author: tmlbl
 */

#ifndef SRC_DB_H_
#define SRC_DB_H_

#include "util/btree.h"
#include "link_node.h"

char *serialize_btree(struct bNode *root, char *out, int *len, int *num);
char *serialize_bnode(struct bNode *node, char *out, int *len, int *num);
void save_db(char *dbf, struct bNode *root);
void load_db(char *dbf, struct bNode *root);
void add_uas(struct bNode *root, char *uas);
int find_uas(struct bNode *root, char *uas);
int find_by_id(struct bNode *root, int id);
void delete_uas(struct bNode *root, char *uas);
void add_class(struct bNode *root, char *uas, char *class);
void remove_class(struct bNode *root, char *uas, char *class);
void print_btree(struct bNode *root);
void print_link_node_int(struct link_node_int *node);

#endif /* SRC_DB_H_ */
