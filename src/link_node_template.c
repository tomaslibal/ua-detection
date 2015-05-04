#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(TEMPLATE_INT)

#    define CONCAT(x, y) x ## y
#    define FUNC(A, B)   CONCAT(A, B)
#    define RENAME(N)    N ## _int
#    define TYPE         int
#    define STRUCT_NAME  RENAME(link_node)

#elif defined(TEMPLATE_FLOAT)

#    define CONCAT(x, y) x ## y
#    define FUNC(A, B)   CONCAT(A, B)
#    define RENAME(N)    N ## _float
#    define TYPE         float
#    define STRUCT_NAME  RENAME(link_node)

#endif

#include "link_node.h"
#define NODE_TYPE float
#include "link_node.h"

/*typedef struct STRUCT_NAME {
    char *name;
    TYPE val;
    struct STRUCT_NAME *next;
} STRUCT_NAME;*/

struct STRUCT_NAME *FUNC(STRUCT_NAME, _create)()
{
	struct STRUCT_NAME *table = NULL;
	table = (struct STRUCT_NAME *) malloc (sizeof(struct STRUCT_NAME));
	if (table == NULL) {
		return NULL;
	}
	table->name = NULL;
	table->val = 0;
	table->next = NULL;

	return table;
}

void FUNC(STRUCT_NAME, _set)(struct STRUCT_NAME *table, char *name, TYPE val)
{
	if (table->name != NULL) {
		free(table->name);
		table->name = NULL;
	}

	if (name == NULL) {
	    return;
	}

	table->name = malloc(sizeof(char) * strlen(name) + 1);

	strcpy(table->name, name);

    table->val = val;
}

void FUNC(STRUCT_NAME, _free)(struct STRUCT_NAME *table)
{
	if (table == NULL) {
		return;
	}

	struct STRUCT_NAME *tmp = NULL;
	struct STRUCT_NAME *tmp_prev = NULL;

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

struct STRUCT_NAME *FUNC(STRUCT_NAME, _get)(struct STRUCT_NAME *head, char *name)
{
	struct STRUCT_NAME *tmp = NULL;

	if (head == NULL || name == NULL) {
		return NULL;
	}

	if (head->name != NULL && strcmp(head->name, name) == 0) {
		return head;
	}

	if (head->next == NULL) {
		return NULL;
	}

	tmp = head->next;

	while(tmp) {
		if (tmp->name != NULL && strcmp(tmp->name, name) == 0) {
			return tmp;
		}
		tmp = tmp->next;
	}

	return NULL;
}

TYPE FUNC(STRUCT_NAME, _get_val)(struct STRUCT_NAME *table, char *name)
{
	struct STRUCT_NAME *tmp = NULL;

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

struct STRUCT_NAME *FUNC(STRUCT_NAME, _get_last)(struct STRUCT_NAME *head)
{
	struct STRUCT_NAME *last = NULL;

	if (head == NULL) {
		return NULL;
	}

	if (head->next == NULL) {
		return head;
	}

	last = head->next;

	while(last->next) {
		last = last->next;
	}

	return last;
}

TYPE FUNC(STRUCT_NAME, _sum_val_rec)(struct STRUCT_NAME *node)
{
    TYPE sum = 0;
    struct STRUCT_NAME *iterator = NULL;
    struct STRUCT_NAME *aux      = NULL;

    if (node == NULL) {
        return sum;
    }

    iterator = node;

    while(iterator) {
        sum      += iterator->val;
        iterator  = iterator->next;
    }

    return sum;
}

#undef RENAME
#undef TYPE
#undef STRUCT_NAME
#undef CONCAT
#undef FUNC