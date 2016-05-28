/*
 * File:   ngramBuilderTest.cpp
 * Author: tlibal
 *
 * Created on Dec 28, 2015, 4:14:15 PM
 */

#include "ngramBuilderTest.h"


CPPUNIT_TEST_SUITE_REGISTRATION(ngramBuilderTest);

ngramBuilderTest::ngramBuilderTest() {
}

ngramBuilderTest::~ngramBuilderTest() {
}

void ngramBuilderTest::setUp() {
    ngBuilder = new NgramBuilder();
}

void ngramBuilderTest::tearDown() {
    delete ngBuilder;
}

void ngramBuilderTest::testLevelDefaultsToThree()
{
    CPPUNIT_ASSERT(ngBuilder->get_level() == 3);
}

void ngramBuilderTest::testSetAndGetLevel()
{
    ngBuilder->set_level(5);
    
    CPPUNIT_ASSERT(ngBuilder->get_level() == 5);
    
    ngBuilder->set_level(1);
    
    CPPUNIT_ASSERT(ngBuilder->get_level() == 1);
}

void ngramBuilderTest::testCannotSetNegativeLevel()
{
    ngBuilder->set_level(-1);
    
    CPPUNIT_ASSERT(ngBuilder->get_level() == 3);
}


void ngramBuilderTest::testFromTokenListBuildsNgrams() 
{
    std::vector<Ngram> ngrams;
    std::string sentence = "Mozilla/5.0 (X11; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0";
    
    ngBuilder->fromUserAgentString(sentence, &ngrams);
    
    /*
     * There are 8 tokens, so 6 ngrams of length 3. The last two tokens don't have enough tokens ahead to for a 3-gram.
     */
    CPPUNIT_ASSERT(ngrams.size() == 6);
    
    Ngram exp1;
    exp1.len = 3;
    exp1.tokens[0] = "Mozilla/5.0";
    exp1.tokens[1] = "X11";
    exp1.tokens[2] = "Linux";
    
    CPPUNIT_ASSERT(ngramMatcher(exp1, ngrams[0]) == true);
    
    Ngram exp2;
    exp2.len = 3;
    exp2.tokens[0] = "X11";
    exp2.tokens[1] = "Linux";
    exp2.tokens[2] = "x86_64";
    
    CPPUNIT_ASSERT(ngramMatcher(exp2, ngrams[1]) == true);
    
    Ngram exp3;
    exp3.len = 3;
    exp3.tokens[0] = "Linux";
    exp3.tokens[1] = "x86_64";
    exp3.tokens[2] = "rv";
    
    CPPUNIT_ASSERT(ngramMatcher(exp3, ngrams[2]) == true);
    
    Ngram exp4;
    exp4.len = 3;
    exp4.tokens[0] = "x86_64";
    exp4.tokens[1] = "rv";
    exp4.tokens[2] = "44.0";
    
    CPPUNIT_ASSERT(ngramMatcher(exp4, ngrams[3]) == true);
    
    Ngram exp5;
    exp5.len = 3;
    exp5.tokens[0] = "rv";
    exp5.tokens[1] = "44.0";
    exp5.tokens[2] = "Gecko/20100101";
    
    CPPUNIT_ASSERT(ngramMatcher(exp5, ngrams[4]) == true);
    
    Ngram exp6;
    exp6.len = 3;
    exp6.tokens[0] = "44.0";
    exp6.tokens[1] = "Gecko/20100101";
    exp6.tokens[2] = "Firefox/44.0";
    
    CPPUNIT_ASSERT(ngramMatcher(exp6, ngrams[5]) == true);
}

bool ngramBuilderTest::ngramMatcher(Ngram& expected, Ngram& actual)
{
    return expected.equals(actual);
}


