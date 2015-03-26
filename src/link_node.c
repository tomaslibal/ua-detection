#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "link_node.h"

struct HTABLE_NAME *AUX(HTABLE_NAME, _create)()
{
	struct HTABLE_NAME *table = NULL;
	table = (struct HTABLE_NAME *) malloc (sizeof(struct HTABLE_NAME));
	if (table == NULL) {
		return NULL;
	}
	table->name = NULL;
	table->val = 0;
	table->next = NULL;

	return table;
}

void AUX(HTABLE_NAME, _set)(struct HTABLE_NAME *table, char *name, HTABLE_TYPE val)
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

void AUX(HTABLE_NAME, _free)(struct HTABLE_NAME *table)
{
	if (table == NULL) {
		return;
	}

	struct HTABLE_NAME *tmp = NULL;
	struct HTABLE_NAME *tmp_prev = NULL;

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

struct HTABLE_NAME *AUX(HTABLE_NAME, _get)(struct HTABLE_NAME *root, char *name)
{
	struct HTABLE_NAME *tmp = NULL;

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

HTABLE_TYPE AUX(HTABLE_NAME, _get_val)(struct HTABLE_NAME *table, char *name)
{
	struct HTABLE_NAME *tmp = NULL;

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

struct HTABLE_NAME *AUX(HTABLE_NAME, _get_last)(struct HTABLE_NAME *root)
{
	struct HTABLE_NAME *last = NULL;

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

HTABLE_TYPE AUX(HTABLE_NAME, _sum_val_rec)(struct HTABLE_NAME *node)
{
    HTABLE_TYPE sum = 0;
    struct HTABLE_NAME *iterator = NULL;
    struct HTABLE_NAME *aux = NULL;

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
