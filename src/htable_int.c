#include <stdio.h>
#include <stdlib.h>

#include "htable_int.h"

struct htable_int *htable_int_create()
{
	struct htable_int *table = NULL;
	table = (struct htable_int *) malloc (sizeof(struct htable_int));
	if (table == NULL) {
		return NULL;
	}
	table->name = NULL;
	table->val = 0;
	table->next = NULL;

	return table;
}

void htable_int_set(struct htable_int *table, char *name, int val)
{
	if (table->name != NULL) {
		free(table->name);
	}
	if (table->name == NULL) {
		table->name = malloc(sizeof(char) * strlen(name));
	}

	strcpy(table->name, name);

	table->val = val;
}

void htable_int_free(struct htable_int *table)
{
	if (table == NULL) {
		return;
	}

	struct htable_int *tmp = NULL;
	struct htable_int *tmp_prev = NULL;

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

struct htable_int *htable_int_get(struct htable_int *root, char *name)
{
	struct htable_int *tmp = NULL;

	if (root == NULL) {
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

int htable_int_get_val(struct htable_int *table, char *name)
{
	struct htable_int *tmp = NULL;

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

struct htable_int *htable_int_get_last(struct htable_int *root)
{
	struct htable_int *last = NULL;

	last = root->next;

	while(last->next) {
		last = last->next;
	}

	return last;
}

