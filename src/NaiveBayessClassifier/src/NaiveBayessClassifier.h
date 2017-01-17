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
    double prob_ngram(Ngram &ngram);
    double prob_category_ngram(std::string const& category, Ngram &ngram);
    int freq_category_word(std::string const& category, std::string const& word);
    
    bool is_in_vocabulary(std::string const& word);
    void stats();
    
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
    
    // make category_vocabularies a map<string, map<string, int>> ...
    std::unordered_map<std::string, std::unordered_map<std::string, int>> category_vocabularies;
    
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

