/* 
 * File:   NgramBuilder.cpp
 * Author: tlibal
 * 
 * Created on November 22, 2015, 7:09 PM
 */

#include <cassert>

#include "NgramBuilder.h"
#include "../../UATokenizer/src/UATokenizer.h"

using std::string;
using std::vector;

string Ngram::toString() {
    string str = "";
    for(int i = 0; i < len; i++) {
        
        str.append(tokens[i]);
        
        if (i < (len-1)) {
            str.append(" ");
        }
    }
    return str;
}

string Ngram::toString(int num) {
    
    assert(len >= num);
    
    string str = "";
    for(int i = 0; i < num; i++) {
        
        str.append(tokens[i]);
        
        if (i < (len-1)) {
            str.append(" ");
        }
    }
    return str;
}

bool Ngram::equals(const Ngram& other)
{
    if (len != other.len) {
        return false;
    }
    
    for(int i = 0; i < len; i++) {
        if (tokens[i].compare(other.tokens[i]) != 0) {
            return false;
        }
    }
    
    return true;
}


NgramBuilder::NgramBuilder() {
}

NgramBuilder::NgramBuilder(const NgramBuilder& orig) {
    level = orig.level;
}

NgramBuilder::~NgramBuilder() {
}

int NgramBuilder::fromTokenList(vector<std::string> &tokens, vector<Ngram> *ngrams) {
    int size = tokens.size();
    /*
     * Offset of the first token to be taken into a new n-gram. Keeps increasing
     * by one with each finished n-gram.
     */
    int slider = 0;

    /*
     * Suppose tokens = 'foo', 'bar', 'baz', 'qux', 'lux'
     * and we're building a 3-grams. Then the 3-grams will be
     * [0] 'foo','bar','baz'
     * [1] 'bar','baz','qux'
     * [2] 'baz','qux','lux'
     */
    
    /*
     * If the dynamic flag is set, we'll progresively build ngrams where n = tokens.size() down to 1
     */
    if (dynamic == true) {
        this->level = size;
    }
    
    for(; slider < size; slider++) {
        /* 
         * The number of remaining tokens must be greater than this->level,
         * otherwise this would not be a valid N-gram of length N. Hence, if it
         * is not the case, we break out from the loop.
         */
        int rem = size - (slider+this->level);
        if (rem < 0) {
            break;
        }

        Ngram n;
        n.len = level;

        for(int j = 0; j < this->level; j++) {
            n.tokens[j] = tokens.at(slider + j);
        }
        
        ngrams->push_back(n);
        
        /*
         * If the dynamic flag is set, here is where we decrease the level number so that
         * after each loop iteration the level is 1 less than in the previous iteration.
         */
        if (dynamic == true) {
            this->level -= 1;
        }
    }
    
    return slider;
}

void NgramBuilder::set_level(const int level)
{
    if (level > 0) {
        this->level = level;
    }
}

int NgramBuilder::get_level()
{
    return level;
}

bool NgramBuilder::is_dynamic()
{
    return dynamic;
}

void NgramBuilder::set_dynamic(const bool is_dynamic)
{
    dynamic = is_dynamic;
}




