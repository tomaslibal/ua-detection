#include <stddef.h>
#include <math.h>

#include "model/bayes.h"
#include "util/logging.h"
#include "util/memutil.h"
#include "link_node.h"
#define NODE_TYPE float
#include "link_node.h"

float evaluate_cls(struct link_node_int *words, struct uas_record *uas_input, char *class, struct link_node_int *corpusDict, struct dict_link_node_int *classDict)
{
    float p_word              = 0;
    float p_word_class        = 0;
    float log_prob            = 0;
    int   corpusDict_word_val = 0;
    
    struct link_node_int      *iterator      = NULL;
    struct link_node_int      *aux           = NULL;
    struct dict_link_node_int *thisClassDict = NULL;
    
    iterator = words;

    /*
     * Loop through each words of the input user-agent string
     */
    while(iterator) {
        /*
         * Look up the frequency of the word in the corpusDict.
         */
        aux = link_node_int_get(corpusDict, iterator->name);

        /*
         * If the word is not in the dictionary there is no data on its prior
         * probability so the program skips the loop
         */
        if (aux == NULL) {
            iterator = iterator->next;
            continue;
        }

        /*
         * Calculate the probability that the word appears at all (in all
         * known user-agent strings).
         */
        p_word = (float)aux->val / (float)link_node_int_sum_val_rec(corpusDict);

        LOGM("P(%s) = %d / %d = %f\n", iterator->name, aux->val, link_node_int_sum_val_rec(corpusDict), p_word);

        /*
         * Now look up the dictionary of the given class
         */
        thisClassDict = dict_link_node_int_find(classDict, class);

        if (thisClassDict == NULL) {
            printf("class dictionary not found for %s\n", class);
            iterator = iterator->next;
            continue;
        }

        /*
         * Look up the frequency of the word in the class' dictionary
         */
        aux = link_node_int_get(thisClassDict->root, iterator->name);
        if (aux == NULL) {
            iterator = iterator->next;
            continue;
        }
        /*
         * Calculate the probability that the word appears in the given
         * class.
         */
        p_word_class = (float)aux->val / (float)link_node_int_sum_val_rec(thisClassDict->root);

        LOGM("P(%s|%s) = %d / %d = %f\n", iterator->name, class, aux->val, link_node_int_sum_val_rec(thisClassDict->root), p_word_class);

        
        if (p_word_class > 0) {
            log_prob += logf((float)iterator->val * p_word_class / p_word);
        }

        iterator = iterator->next;
    }

    return log_prob;
}

/*
 * Evaluates user input:
 *
 *     *words: tokenized keywords of the user-agent string
 *     *uas_input: the user-agent string and the class
 */
float evaluate(struct link_node_int *words, struct uas_record *uas_input, struct link_node_int *corpusDict, struct dict_link_node_int *classDict)
{
    float p_word = 0;
    float p_word_class = 0;
    float log_prob_word_class = 0.0;
    int corpusDict_word_val = 0;
    
    struct dict_link_node_int *thisClassDict = NULL;
    struct link_node_int *aux = NULL;
    struct link_node_int *iterator;

    iterator = words;

    /*
     * Loop through each words of the input user-agent string
     */
    while(iterator) {
        /*
         * Look up the frequency of the word in the corpusDict.
         */
        aux = link_node_int_get(corpusDict, iterator->name);

        /*
         * If the word is not in the dictionary there is no data on its prior
         * probability so the program skips the loop
         */
        if (aux == NULL) {
            iterator = iterator->next;
            continue;
        }

        /*
         * Calculate the probability that the word appears at all (in all
         * known user-agent strings).
         */
        p_word = (float)aux->val / (float)link_node_int_sum_val_rec(corpusDict);

        LOGM("P(%s) = %d / %d = %f\n", iterator->name, aux->val, link_node_int_sum_val_rec(corpusDict), p_word);

        /*
         * Now look up the dictionary of the given class
         */
        thisClassDict = dict_link_node_int_find(classDict, uas_input->class);

        if (thisClassDict == NULL) {
            printf("class dictionary not found for %s\n", uas_input->class);
            iterator = iterator->next;
            continue;
        }

        /*
         * Look up the frequency of the word in the class' dictionary
         */
        aux = link_node_int_get(thisClassDict->root, iterator->name);
        if (aux == NULL) {
            iterator = iterator->next;
            continue;
        }
        /*
         * Calculate the probability that the word appears in the given
         * class.
         */
        p_word_class = (float)aux->val / (float)link_node_int_sum_val_rec(thisClassDict->root);

        LOGM("P(%s|%s) = %d / %d = %f\n", iterator->name, uas_input->class, aux->val, link_node_int_sum_val_rec(thisClassDict->root), p_word_class);

        if (p_word_class > 0) {
            log_prob_word_class += logf((float)iterator->val * p_word_class / p_word);
        }

        iterator = iterator->next;
    }
    
    return log_prob_word_class;
}

void cmp_all(struct link_node_int *prior, struct link_node_int *words, struct uas_record *uas_input, struct link_node_float *p_prior)
{
    // each class must have a *.cls.txt file in the data folder
    // so list that folder and evaluate uas_input for each class
    // found in that directory

    // for now, manually override to mobile,desktop,android
    struct link_node_int *tmp = NULL;
    struct link_node_int *cls_iterator = NULL;
    float prior_class_val = 0;
    float log_prob = 0;
    float a = 0.0;
    float b = 0.0;
    struct link_node_float *aux_float = NULL;
    int lc = 0;
    struct uas_record *head = uas_record_create();
    struct link_node_int *corpusDict = link_node_int_create();
    struct dict_link_node_int *classDict = dict_link_node_int_create();

    cls_iterator = link_node_int_create();
    link_node_int_set(cls_iterator, "mobile", 0);
    tmp = link_node_int_create();
    link_node_int_set(tmp, "desktop", 0);
    cls_iterator->next = tmp;
    tmp = link_node_int_create();
    link_node_int_set(tmp, "android", 0);
    cls_iterator->next->next = tmp;
    tmp = cls_iterator;

    while(cls_iterator) {
        uas_record_free(head);
        head = uas_record_create();
        chck_malloc((void *) head, "Array of UAS Records");
        read_cls_data(cls_iterator->name, head, &lc);
        train(head, prior, corpusDict, classDict, p_prior);

        prior_class_val = 0;
        aux_float = link_node_float_get(p_prior, cls_iterator->name);
        if (aux_float != NULL)
            prior_class_val = aux_float->val;

        log_prob = evaluate_cls(words, uas_input, cls_iterator->name, corpusDict, classDict);

        a = expf(log_prob + logf(prior_class_val));

        prior_class_val = 0;
        aux_float = link_node_float_get(p_prior, "other");
        if (aux_float != NULL)
            prior_class_val = aux_float->val;

        log_prob = evaluate_cls(words, uas_input, "other", corpusDict, classDict);

        b = expf(log_prob + logf(prior_class_val));

	LOGM("[cls:input] %s\n", uas_input->uas);
        LOGM("[cls:output %s] = %f\n", cls_iterator->name, a);
        LOGM("[cls:output non-%s] = %f\n", cls_iterator->name, b);
	
        printf("[cls:%s]=%f%%\n", cls_iterator->name, (a / (a+b))*100);

        cls_iterator = cls_iterator->next;
    }

    link_node_int_free(tmp);
}

/**
 * Evaluates a given User-Agent string against a given classifier
 *
 * uas_record *uas_input should contain the User-Agent string and the class
 * link_node_int *words bag-of-words from the User-Agent string
 */
void cmp_one(struct link_node_int *words, struct uas_record *uas_input, struct link_node_int *corpusDict, struct dict_link_node_int *classDict, struct link_node_float *p_prior)
{
    float prior_class_val = 0.0;
    float log_prob = 0.0;
    float a = 0.0;
    float b = 0.0;
    float log_prob_word_class = 0.0;
    struct link_node_float *aux_float = NULL;

    /*
     * VI. Evaluate the user input
     */
    log_prob_word_class = evaluate(words, uas_input, corpusDict, classDict);

    /*
     * Print the result:
     */

    aux_float = link_node_float_get(p_prior, uas_input->class);

    // if found
    if (aux_float != NULL)
        prior_class_val = aux_float->val;

    a = expf(log_prob_word_class + logf(prior_class_val));
    LOGM("[cls:input] %s\n", uas_input->uas);
    LOGM("[cls:output %s] = %f\n", uas_input->class, a);

    // -vs-all:

    prior_class_val = 0;
    aux_float = link_node_float_get(p_prior, "other");
    if (aux_float != NULL)
        prior_class_val = aux_float->val;

    log_prob = evaluate_cls(words, uas_input, "other", corpusDict, classDict);

    b = expf(log_prob + logf(prior_class_val));
    LOGM("[cls:output non-%s] = %f\n", uas_input->class, b);

    printf("[cls:%s]=%f%%\n", uas_input->class, (a / (a+b))*100);
}