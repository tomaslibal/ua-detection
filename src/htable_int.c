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
