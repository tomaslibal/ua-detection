/* 
 * File:   NgramBuilder.cpp
 * Author: tlibal
 * 
 * Created on November 22, 2015, 7:09 PM
 */

#include <cassert>
#include <functional>
#include <exception>

#include "NgramBuilder.h"
#include "../../UATokenizer/src/UATokenizer.h"

using std::string;
using std::vector;

string NgramSimple::toString()
{
    return sentence;
}

string NgramSimple::toString(int num)
{
    assert(len >= num);
    
    assert(num > 0);
    
    int start = starts[0];
    int len = 0;
    
    for (int i = 0; i <= num-1; i++) {
        len += lens[i];
    }
    
    return sentence.substr(start, len + num -1);
}


bool NgramSimple::equals(const NgramSimple& other)
{
    return sentence.compare(other.sentence) == 0;
}

unsigned int NgramSimple::hash()
{
    return std::hash<std::string>{} (sentence);
}




NgramBuilder::NgramBuilder() {
}

NgramBuilder::NgramBuilder(const NgramBuilder& orig) {
    level = orig.level;
}

NgramBuilder::~NgramBuilder() {
}

int NgramBuilder::fromTokenList(char** tokens, const int numTokens, vector< NgramSimple >* ngrams)
{
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
    
    if (level > NGRAM_MAX_SIZE) {
        throw NgramSimpleLengthExceededException("NgramSimple supports max len NGRAM_MAX_SIZE, was " + std::to_string(level));
    }
    
    /*
     * If the dynamic flag is set, we'll progresively build ngrams where n = tokens.size() down to 1
     */
    if (dynamic == true) {
        if (numTokens > NGRAM_MAX_SIZE) {
            this->level = NGRAM_MAX_SIZE;
        } else {
            this->level = numTokens;
        }
    }

    for(; slider < numTokens; slider++) {
        /* 
         * The number of remaining tokens must be greater than this->level,
         * otherwise this would not be a valid N-gram of length N. Hence, if it
         * is not the case, we break out from the loop.
         */
        int rem = numTokens - (slider+this->level);
        if (rem < 0) {
            break;
        }

        NgramSimple n;
        n.len = level;
        
        string sentence;

        for(int j = 0; j < this->level; j++) {
            if ((slider + j) >= numTokens) { break; }
            string token = tokens[slider + j];
            n.starts[j] = sentence.size();
            n.lens[j] = token.size();            
            sentence.append(token);
            
            if (j < (this->level)-1) {
                sentence.append(" ");
            }
        }
        n.sentence = sentence;
        
        ngrams->push_back(n);
        
         /*
         * If the dynamic flag is set, here is where we decrease the level number so that
         * after each loop iteration the level is 1 less than in the previous iteration.
         */
        if (dynamic == true) {
            this->level -= 1;
        }
    }
    
    for (int i = 0; i < numTokens; i++) {
        delete[] tokens[i];
    }
    delete[] tokens;
    tokens = nullptr;
    
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




