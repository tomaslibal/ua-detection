#ifndef _BAYES_H_
#define _BAYES_H_

#include "../reader.h"
#include "../dictionary.h"
#include "../probab.h"

/*
 * The training process has three outcomes:
 *
 *  * learn the probability of a word being in a class, in other words learn
 *    P(<words-in-uas>|class),
 * 
 *  * learn the probability of a class, P(class),
 * 
 *  * learn the probability of a word at all P(word)
 * 
 * Inputs:
 *  - uas_record *head: supplies the training data; it is a linked list head 
 *    of a UAS, class pairs
 *  - link_node_int *prior: keeps count of how many classes and how many time
 *    each class is in the data set
 *  - link_node_int *corpusDict: keeps count on the frequencies of the words
 *    across all classes
 *  - dict_link_node_int *classDict: keeps count on the the frequencies of the 
 *    words separately for each class
 *  - link_node_float *p_prior: relative frequency of each class P(class) 
 */
void train(struct uas_record *head, struct link_node_int *prior, struct link_node_int *corpusDict, struct dict_link_node_int *classDict, struct link_node_float *p_prior);
/*
 * 
 */
float evaluate(struct link_node_int *words, struct uas_record *uas_input, struct link_node_int *corpusDict, struct dict_link_node_int *classDict);
/*
 * 
 */
float evaluate_cls(struct link_node_int *words, struct uas_record *uas_input, char *class, struct link_node_int *corpusDict, struct dict_link_node_int *classDict);
/*
 * 
 */
void cmp_all(struct link_node_int *prior, struct link_node_int *words, struct uas_record *uas_input, struct link_node_float *p_prior);
/*
 * 
 */
void cmp_one(struct link_node_int *words, struct uas_record *uas_input, struct link_node_int *corpusDict, struct dict_link_node_int *classDict, struct link_node_float *p_prior);

#endif