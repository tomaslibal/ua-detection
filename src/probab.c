#include <stdio.h>
#include <stdlib.h>

#include "probab.h"

void count_words(struct uas_record *datapoint, struct link_node_int *store)
{
	struct link_node_int *tmp = NULL;
	struct link_node_int *aux = NULL;

	char* tokens[64];

	int number;
	int* p_number = &number;

	if (datapoint == NULL) {
		return;
	}

	/*
	 * `store` = dictionary of words, e.g.:
	 *     store['Mozilla'] = 1 ... 1 occurence of word 'Mozilla' in the user
	 *     agent string
	 */
	if (store == NULL) {
		return;
	}

	if(tok(datapoint->uas, tokens, p_number) != 0) {
		printf("Error executing 'tok'\n");
	}

	/*
	 * For each word in the user-agent string:
	 *     if dict[word] is null
	 *         dict[word] = 1
	 *     else
	 *     	   dict[word] += 1
	 */
	for(int i = 0; i < number; i++) {
		if (i == 0) {
			link_node_int_set(store, tokens[0], 1);
		} else {
			tmp = link_node_int_get(store, tokens[i]);

			if (tmp == NULL) {
				tmp = link_node_int_create();
				link_node_int_set(tmp, tokens[i], 1);
				aux = link_node_int_get_last(store);
				aux->next = tmp;
			} else {
				tmp->val++;
			}
		}

		free(tokens[i]);
	}
}
