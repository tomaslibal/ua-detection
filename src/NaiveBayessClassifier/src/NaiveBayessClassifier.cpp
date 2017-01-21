/* 
 * File:   NaiveBayessClassifier.cpp
 * Author: tlibal
 * 
 * Created on November 22, 2015, 7:10 PM
 */

#include <cmath>
#include <iostream>

#include "NaiveBayessClassifier.h"
#include "../../UATokenizer/src/UATokenizer.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::unordered_map;
using std::pair;

#define INIT_MAP_SIZE 128000

NaiveBayessClassifier::NaiveBayessClassifier() {
    uaNgramBuilder.set_dynamic(true);
    priors_freq.reserve(INIT_MAP_SIZE);
    vocabulary.reserve(INIT_MAP_SIZE);
    cat_vocabulary.reserve(INIT_MAP_SIZE);
    cat_vocabulary_freq.reserve(INIT_MAP_SIZE);
}

NaiveBayessClassifier::~NaiveBayessClassifier() {

}

void NaiveBayessClassifier::inc_priors_freq(string const& key) {
    auto search = priors_freq.find(key);
    if (search != priors_freq.end()) {
        search->second++;
    } else {
        priors_freq.emplace(key, 1);
    }
}

void NaiveBayessClassifier::add_word(string const& word, string const& category) {
    // global vocabulary
    auto voc_search = vocabulary.find(word);
    if (voc_search != vocabulary.end()) {
        voc_search->second++;
    } else {
        vocabulary.emplace<>(word, 1);
    }
    // category's vocabulary:
    std::string cat_and_word = category + word;
    auto search = cat_vocabulary.find(cat_and_word);
    if (search != cat_vocabulary.end()) {
        search->second++;
        cat_vocabulary_freq[category]++;
    } else {
        cat_vocabulary.emplace<>(cat_and_word, 1);
        cat_vocabulary_freq.emplace<>(category, 1);
    }
}

void NaiveBayessClassifier::add_data(string const& data, string const& category) {
    // increase the frequency of this type by 1
    this->inc_priors_freq(category);
    
    // tokenize the data (user-agent string) into ngrams
    vector<NgramSimple> ngrams;
    uaNgramBuilder.fromUserAgentString(data, &ngrams);
 
    for (auto & ngram: ngrams) {
        for(int j = 1; j <= ngram.len; j++) {
            string n = ngram.toString(j);
            add_word(n, category);
        }
    }
   
}

void NaiveBayessClassifier::clear_cache()
{
    cache.int_cache.clear();
    cache.dbl_cache.clear();
}


double NaiveBayessClassifier::prob_category(std::string const& category) {
    int freq_cat = 0;
    std::string category_key = "0123cat" + category;
    
    if (cache.in_dbl_cache(category_key)) {
        return cache.get_dbl_cache(category_key);
    }
    
    auto search = priors_freq.find(category);
    if (search != priors_freq.end()) {
        freq_cat = search->second;
    }
    int freq_all = 0;
    
    if (!cache.in_int_cache("all_freq_categories")) {
        for (auto& pair: priors_freq) {
            freq_all += pair.second;
        }
        cache.insert_int_cache("all_freq_categories", freq_all);
    } else {
        freq_all = cache.get_int_cache("all_freq_categories");
    }
    
    double p = 0;
    
    if (freq_all > 0) {
        p = double(freq_cat)/double(freq_all);
    }
    
    cache.insert_dbl_cache(category_key, p);
    return p;
}

double NaiveBayessClassifier::prob_ngram(NgramSimple& ngram) {
    int freq = 0;
    int ngramLen = ngram.len;
    std::string ngramString = ngram.toString();
    std::string ngram_key = "0123ngram" + ngramString;
    
    if (cache.in_dbl_cache(ngram_key)) {
        return cache.get_dbl_cache(ngram_key);
    }
    
    for(int i = 1; i <= ngramLen; i++) {
        string s = ngram.toString(i);
        auto search = vocabulary.find(s);
        if (search != vocabulary.end()) {
            freq += search->second;
        }
    }  
    
    int all_freq = 0;
    
    if (!cache.in_int_cache("all_freq_ngrams")) {
        for (auto& pair: vocabulary) {
            all_freq += pair.second;
        }
        cache.insert_int_cache("all_freq_ngrams", all_freq);
    } else {
        all_freq = cache.get_int_cache("all_freq_ngrams");
    }    
    
    double p = 0;
    
    if (all_freq > 0) {
        p = double(freq)/double(all_freq);
    }
    
    cache.insert_dbl_cache(ngram_key, p);
    return p;
}

int NaiveBayessClassifier::freq_category_word(std::string const& category, std::string const& word) {
    std::string key = "0123catword" + category + word;
    
    if (cache.in_int_cache(key)) {
        return cache.get_int_cache(key);
    }
    
    std::string cat_and_word = category + word;
    auto search = cat_vocabulary.find(cat_and_word);
    if (search != cat_vocabulary.end()) {
        cache.insert_int_cache(key, search->second);
        return search->second;
    }
    
    cache.insert_int_cache(key, 0);
    return 0;
}

double NaiveBayessClassifier::prob_category_ngram(std::string const& category, NgramSimple& ngram) {
    
    std::string ngramString = ngram.toString();
    std::string key = "0123freqcatngram" + category + ngramString; 
    
    if (cache.in_dbl_cache(key)) {
        return cache.get_dbl_cache(key);
    }
    
    int ngram_freq_in_cat = 0;
    int ngrams_freq_total_in_cat = 0;
        
    int ngramLen = ngram.len;
    for(int i = 1; i <= ngramLen; i++) {
        string s = ngram.toString(i);
        ngram_freq_in_cat += freq_category_word(category, s);
    }
    
    auto search = cat_vocabulary_freq.find(category);
    if (search != cat_vocabulary_freq.end()) {
        ngrams_freq_total_in_cat = search->second;
    } else {
        ngrams_freq_total_in_cat = 0;
    }
    
    double p = 0;
    if (ngrams_freq_total_in_cat > 0) {
        p = double(ngram_freq_in_cat) / double(ngrams_freq_total_in_cat);
    }
    
    cache.insert_dbl_cache(key, p);
    return p;
}

bool NaiveBayessClassifier::is_in_vocabulary(std::string const& word) {
    auto search = vocabulary.find(word);
    if (search != vocabulary.end()) {
        return true;
    } else {
        return false;
    }
}


double NaiveBayessClassifier::classify(std::string const& data, std::string const& category) {
    vector<NgramSimple> ngrams;
    
    uaNgramBuilder.fromUserAgentString(data, &ngrams);
    
    double p_cat = prob_category(category);
    double log_posterior = 0;
    
    for(unsigned int i = 0; i < ngrams.size(); i++) {
        string ns = ngrams[i].toString(ngrams[i].len);
        // discard words 2 chars or shorter
        if (ns.length() < 3) {
            continue;
        }
        
        double p_word = prob_ngram(ngrams[i]);
        
        double p_word_cat = prob_category_ngram(category, ngrams[i]);
        
        if (p_word_cat > 0) {
            log_posterior += std::log(p_word_cat / p_word);
        }
    }
    
    return std::exp( log_posterior + std::log( p_cat ) );
}

void NaiveBayessClassifier::stats() {
    cout << "# categories: " << priors_freq.size() << endl;
    cout << "Words (ngrams) in vocabulary: " << vocabulary.size() << endl;
}

vector<string>* NaiveBayessClassifier::get_categories() {
    vector<string>* cat = new vector<string>();
    
    for(unordered_map<string, int>::iterator it = priors_freq.begin(); it != priors_freq.end(); ++it) {
        cat->push_back(it->first);
    }
    
    return cat;
}
