/* 
 * File:   NaiveBayessClassifier.h
 * Author: tlibal
 *
 * Created on November 22, 2015, 7:10 PM
 */

#ifndef NAIVEBAYESSCLASSIFIER_H
#define	NAIVEBAYESSCLASSIFIER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "../../NgramBuilder/src/UaNgramBuilder.h"
#include "./NBCCache.h"

class NaiveBayessClassifier {
public:
    NaiveBayessClassifier();
    virtual ~NaiveBayessClassifier();
    
    void add_word(std::string const& word, std::string const& category);
    void add_data(std::string const& data, std::string const& category);
    
    double classify(std::string const& data, std::string const& category);
    
    double prob_category(std::string const& category);
    double prob_ngram(NgramSimple &ngram);
    double prob_category_ngram(std::string const& category, NgramSimple &ngram);
    int freq_category_word(std::string const& category, std::string const& word);
    
    void stats();
    void clear_cache();
    
    std::vector<std::string>* get_categories();
private:
    NBCCache cache;
    /*
     * prior for categories (frequencies):
     *     cat1 = 42,
     *     cat2 =  4,
     *     ...
     *     catN =  y
     */
//    ChainedHashTable<int> priors_freq;
    std::unordered_map<std::string, int> priors_freq;
    
    /*
     * vocabulary of all seen words with their freqeuencies
     *     word1 = 53109,
     *     word2 =  4021,
     *     ...
     *     wordN =  9211
     */
    std::unordered_map<std::string, int> vocabulary;

    /*
     * Let c = "mobile" be the category and w1 = "Android", w2 = "EN" and w3 = "EN" be the words in this category,
     * then the "mobile" category vocabulary will be "mobile_Android"=1, "mobile_EN"=2.
     */
    std::unordered_map<std::string, int> cat_vocabulary;
    /*
     * this auxiliary map keeps the number of occurences of words in the vocabulary for each given category,
     * so for the example above in cat_vocabulary, the size for "mobile" will be 3.
     */
    std::unordered_map<std::string, int> cat_vocabulary_freq;
    
    /*
     * increments a frequency of a prior (key) by one
     */
    void inc_priors_freq(std::string const& key);
    
    /*
     * Tokenizes user-agent string and builds the ngrams from the tokens
     */
    UaNgramBuilder uaNgramBuilder;

};

#endif	/* NAIVEBAYESSCLASSIFIER_H */

