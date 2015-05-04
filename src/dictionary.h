#ifndef DICTIONARY
#define DICTIONARY

#include "link_node.h"
#define NODE_TYPE float
#include "link_node.h"

struct dict_link_node_int {
    char *class_name;
    struct link_node_int *root;
    struct dict_link_node_int *next;
};

struct dict_link_node_int *dict_link_node_int_create();

void dict_link_node_int_set(
        struct dict_link_node_int *node,
        char *class_name,
        struct link_node_int *root,
        struct dict_link_node_int *next);

struct dict_link_node_int *dict_link_node_int_find(struct dict_link_node_int *node, char *class_name);
struct dict_link_node_int *dict_link_node_int_find_last(struct dict_link_node_int *node);
void dict_link_node_int_free(struct dict_link_node_int *node);

#endif
