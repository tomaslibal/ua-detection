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

	struct htable_int *tmp, *tmp_prev;

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

int htable_int_get(struct htable_int *table, char *name)
{
	struct htable_int *tmp;

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


