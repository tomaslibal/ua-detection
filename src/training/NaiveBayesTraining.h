/* 
 * File:   NaiveBayesTraining.h
 * Author: tomaslibal
 *
 * Created on 18 October 2015, 12:15
 */

#ifndef NAIVEBAYESTRAINING_H
#define	NAIVEBAYESTRAINING_H

#include "../ngram/NgramBuilder.h"

typedef struct dictionary {
    int len;
    char** keys;
    double *values;
} dictionary;

typedef struct dictionary_list {
    dictionary *item;
    dictionary_list *next;
} dictionary_list;

class NaiveBayesTraining {
public:
    NaiveBayesTraining();
    NaiveBayesTraining(const NaiveBayesTraining& orig);
    virtual ~NaiveBayesTraining();
    
    /*
     * index is a dictionary of all accounted words that have been seen so far
     * i.e.
     * index = {
     *     "Mozilla": 320.0,
     *     ...
     * }
     */
    dictionary *index;
    
    dictionary_list *ngram_priors_categories;
    
    dictionary *category_priors;
    
    void count_ngrams(Ngram *ng, dictionary *counts);
    
    /*
     * initializes ngram_priors_categories, category_priors and index (token
     * priors).
     * 
     * ngram_priors_categories = {
     *     "Mobile": {
     *         ["Mozilla", "Android", "Maker"]: 0.073,
     *         ...
     *     },
     *     "OtherCategory": { ... }
     * }
     * 
     * category_priors = {
     *     "Mobile": 0.5,
     *     "OtherCategory": 0.5,
     *     ...
     * }
     * 
     * index - see above
     */
    void init();
    
    void update(Ngram *ngram, const char *category);
    
    /*
     * move out to a new class NaiveBayesPredict
     */
    void eval(const char *sentence, char *output);
private:

};

#endif	/* NAIVEBAYESTRAINING_H */

