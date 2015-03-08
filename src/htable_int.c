#include <stdio.h>
#include <stdlib.h>

#include "htable_int.h"

struct htable_int *htable_int_create()
{
	struct htable_int *table;
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

