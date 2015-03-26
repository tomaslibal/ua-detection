#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "link_node.h"

struct dict_htable_int *dict_htable_int_create()
{
    struct dict_htable_int *node = NULL;
    node = malloc(sizeof(struct dict_htable_int));
    if(node == NULL) {
        return NULL;
    }

    node->class_name = NULL;
    node->next = NULL;
    node->root = NULL;

    return node;
}

void dict_htable_int_set(struct dict_htable_int *node, char *class_name, struct htable_int *root, struct dict_htable_int *next)
{
    if (node == NULL) {
        return;
    }

    if (node->class_name != NULL) {
        free(node->class_name);
    }

    node->class_name = malloc(sizeof(char) * strlen(class_name) + 1);
    strcpy(node->class_name, class_name);

    node->root = root;

    node->next = next;
}
struct dict_htable_int *dict_htable_int_find(struct dict_htable_int *node, char *class_name)
{
    struct dict_htable_int *iterator = NULL;

    if (node == NULL) {
        return NULL;
    }

    iterator = node;

    while(iterator) {
        if (iterator->class_name != NULL && strcmp(iterator->class_name, class_name) == 0) {
            break;
        }
        iterator = iterator->next;
    }

    return iterator;
}

void dict_htable_int_free(struct dict_htable_int *node)
{
    struct dict_htable_int *iterator = NULL;

    if (node == NULL) {
        return;
    }

    if (node->class_name != NULL) {
        free(node->class_name);
    }

    if (node->root != NULL) {
        htable_int_free(node->root);
    }

    dict_htable_int_free(node->next);
    free(node);
}

struct dict_htable_int *dict_htable_int_find_last(struct dict_htable_int *root)
{
    struct dict_htable_int *last = NULL;

    if (root == NULL) {
        return NULL;
    }

    if (root->next == NULL) {
        return root;
    }

    last = root->next;

    while(last->next) {
        last = last->next;
    }

    return last;
}
