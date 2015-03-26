#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "link_node.h"

struct LINK_NODE_NAME *AUX(LINK_NODE_NAME, _create)()
{
	struct LINK_NODE_NAME *table = NULL;
	table = (struct LINK_NODE_NAME *) malloc (sizeof(struct LINK_NODE_NAME));
	if (table == NULL) {
		return NULL;
	}
	table->name = NULL;
	table->val = 0;
	table->next = NULL;

	return table;
}

void AUX(LINK_NODE_NAME, _set)(struct LINK_NODE_NAME *table, char *name, NODE_TYPE val)
{
	if (table->name != NULL) {
		free(table->name);
	}

	if (name == NULL) {
	    return;
	}

	table->name = malloc(sizeof(char) * strlen(name) + 1);

	strcpy(table->name, name);

	table->val = val;
}

void AUX(LINK_NODE_NAME, _free)(struct LINK_NODE_NAME *table)
{
	if (table == NULL) {
		return;
	}

	struct LINK_NODE_NAME *tmp = NULL;
	struct LINK_NODE_NAME *tmp_prev = NULL;

	tmp = table;

	while(tmp != NULL) {
		if (tmp->name != NULL) {
			free(tmp->name);
		}
		tmp_prev = tmp;
		tmp = tmp->next;

		free(tmp_prev);
	}
}

struct LINK_NODE_NAME *AUX(LINK_NODE_NAME, _get)(struct LINK_NODE_NAME *root, char *name)
{
	struct LINK_NODE_NAME *tmp = NULL;

	if (root == NULL || name == NULL) {
		return NULL;
	}

	if (root->name != NULL && strcmp(root->name, name) == 0) {
		return root;
	}

	if (root->next == NULL) {
		return NULL;
	}

	tmp = root->next;

	while(tmp) {
		if (tmp->name != NULL && strcmp(tmp->name, name) == 0) {
			return tmp;
		}
		tmp = tmp->next;
	}

	return NULL;
}

NODE_TYPE AUX(LINK_NODE_NAME, _get_val)(struct LINK_NODE_NAME *table, char *name)
{
	struct LINK_NODE_NAME *tmp = NULL;

	if (table == NULL) {
		return 0;
	}

	if (table->name != NULL && strcmp(table->name, name) == 0) {
		return table->val;
	}

	if (table->next == NULL) {
		return 0;
	}

	tmp = table->next;

	while(tmp) {
		if (tmp->name != NULL && strcmp(tmp->name, name) == 0) {
			return tmp->val;
		}
		tmp = tmp->next;
	}

	return 0;
}

struct LINK_NODE_NAME *AUX(LINK_NODE_NAME, _get_last)(struct LINK_NODE_NAME *root)
{
	struct LINK_NODE_NAME *last = NULL;

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

NODE_TYPE AUX(LINK_NODE_NAME, _sum_val_rec)(struct LINK_NODE_NAME *node)
{
    NODE_TYPE sum = 0;
    struct LINK_NODE_NAME *iterator = NULL;
    struct LINK_NODE_NAME *aux = NULL;

    if (node == NULL) {
        return sum;
    }

    iterator = node;

    while(iterator) {
        sum += iterator->val;
        iterator = iterator->next;
    }

    return sum;
}
