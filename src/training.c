#include <stddef.h>

#include "model/bayes.h"
#include "util/logging.h"

#include "link_node.h"
#define NODE_TYPE float
#include "link_node.h"

/*
 * struct uas_record *root: pass the root node of the uas_record linked list 
 * containing UA strings.
 */
void train(struct uas_record *root, struct link_node_int *prior, struct link_node_int *corpusDict, struct dict_link_node_int *classDict, struct link_node_float *p_prior)
{
    struct uas_record         *record        = root;
    struct link_node_int      *tmp           = NULL;
    struct link_node_int      *aux           = NULL;
    struct dict_link_node_int *thisClassDict = NULL;
    struct dict_link_node_int *tmpDict       = NULL;
    struct dict_link_node_int *auxDict       = NULL;
    struct link_node_int      *iterator      = NULL;
    struct link_node_float    *p_iterator    = NULL;

    /*
     * (This assumes that there are at least 2 records, else the test
     * in the while loop will evaluate to false in the first pass already).
     */
    while (record->next) {
        /*
         * record the number of record->class that have been read:
         *
         * lookup the struct for the record->class and +1 its val
         * field. If not found, it's the first time this class has been
         * read so create a new struct and append it to the end link...
         */
        tmp = link_node_int_get(prior, record->class);

        if (tmp) {
            tmp->val++;
        } else if (prior->name == NULL) {
            link_node_int_set(prior, record->class, 1);
        } else {
            tmp = link_node_int_get_last(prior);
            aux = link_node_int_create();
            link_node_int_set(aux, record->class, 1);
            tmp->next = aux;
        }

        /*
         * Increment the counters for each unique word # of occurrences
         */
        count_words(record, corpusDict);

        /*
         * Do the same for the class dictionary. If the class dictionary not
         * found, create a new one as we have a new class
         */
        thisClassDict = dict_link_node_int_find(classDict, record->class);

        if (thisClassDict) {
            count_words(record, thisClassDict->root);
        } else if (classDict->root == NULL) {
            aux = link_node_int_create();
            dict_link_node_int_set(classDict, record->class, aux, NULL);
            count_words(record, classDict->root);
        } else {
            tmpDict = dict_link_node_int_create();
            aux = link_node_int_create();
            dict_link_node_int_set(tmpDict, record->class, aux, NULL);
            count_words(record, tmpDict->root);

            auxDict = dict_link_node_int_find_last(classDict);
            auxDict->next = tmpDict;
        }

        record = record->next;
    }

    iterator = prior;
    p_iterator = p_prior;

    int sum_prior_vals = link_node_int_sum_val_rec(prior);
    float val;

    LOGM("sum_prior_vals=%d\n", sum_prior_vals);
    
    while(iterator) {
        LOGM("prior: class %s, cnt %d\n", iterator->name, iterator->val);
        val = (float)iterator->val / (float)sum_prior_vals;

        link_node_float_set(p_iterator, iterator->name, val);
        p_iterator->next = link_node_float_create();
        LOGM("P(%s) = %d / %d = %f\n", p_iterator->name, iterator->val, sum_prior_vals, p_iterator->val);

        p_iterator = p_iterator->next;
        iterator = iterator->next;
    }
}