/*
 * File:   ngramBuilderTest.cpp
 * Author: tlibal
 *
 * Created on Dec 28, 2015, 4:14:15 PM
 */

#include "ngramBuilderTest.h"

#include <string>
#include <vector>
#include <iostream>

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
    
    std::vector<std::string> tokens;
    tokens.push_back("Mozilla/5.0");
    tokens.push_back("X11");
    tokens.push_back("Linux");
    tokens.push_back("x86_64");
    
    ngBuilder->fromTokenList(tokens, &ngrams);
    
    /*
     * There are 4 tokens, so 2 ngrams of length 3 can be created. The last two tokens don't have enough tokens ahead to be able to form a 3-gram.
     */
    CPPUNIT_ASSERT(ngrams.size() == 2);
    
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
    
}

void ngramBuilderTest::testNgramSimple()
{
    std::vector<NgramSimple> ngrams;
    
    std::vector<std::string> tokens;
    tokens.push_back("Mozilla/5.0");
    tokens.push_back("X11");
    tokens.push_back("Linux");
    tokens.push_back("x86_64");
    
    ngBuilder->fromTokenList(tokens, &ngrams);
    
    CPPUNIT_ASSERT(ngrams.size() == 2);
    
    NgramSimple exp1;
    exp1.len = 3;
    exp1.sentence = "Mozilla/5.0 X11 Linux ";
    exp1.starts[0] = 0, exp1.starts[1] = 12; exp1.starts[2] = 16;
    exp1.lens[0] = 11; exp1.lens[1]= 3; exp1.lens[2] = 5;
    
    CPPUNIT_ASSERT(ngramSimpleMatcher(exp1, ngrams.at(0)) == true);
    
    CPPUNIT_ASSERT_EQUAL(exp1.starts[0], ((NgramSimple) ngrams.at(0)).starts[0]);
    CPPUNIT_ASSERT_EQUAL(exp1.starts[1], ((NgramSimple) ngrams.at(0)).starts[1]);
    CPPUNIT_ASSERT_EQUAL(exp1.starts[2], ((NgramSimple) ngrams.at(0)).starts[2]);
    
    CPPUNIT_ASSERT_EQUAL(exp1.lens[0], ((NgramSimple) ngrams.at(0)).lens[0]);
    CPPUNIT_ASSERT_EQUAL(exp1.lens[1], ((NgramSimple) ngrams.at(0)).lens[1]);
    CPPUNIT_ASSERT_EQUAL(exp1.lens[2], ((NgramSimple) ngrams.at(0)).lens[2]);
}


void ngramBuilderTest::testSetDynamicFlag()
{
    CPPUNIT_ASSERT_EQUAL(false, ngBuilder->is_dynamic());
    
    ngBuilder->set_dynamic(true);
        
    CPPUNIT_ASSERT_EQUAL(true, ngBuilder->is_dynamic());
    
    ngBuilder->set_dynamic(false);
    
    CPPUNIT_ASSERT_EQUAL(false, ngBuilder->is_dynamic());
}


void ngramBuilderTest::testZeroLevelMeansMaxPossibleNgram()
{
    std::vector<Ngram> ngrams;
    
    std::vector<std::string> tokens;
    tokens.push_back("Mozilla/5.0");
    tokens.push_back("X11");
    tokens.push_back("Linux");
    tokens.push_back("x86_64");
    tokens.push_back("Firefox/60.0");
    
    
    ngBuilder->set_dynamic(true);
    ngBuilder->fromTokenList(tokens, &ngrams);
    
    // There should be 5 ngrams, 1 of length 5, 1 of length 4, 1 of length 3, 1 of length 2 and 1 of length 1
    CPPUNIT_ASSERT_EQUAL(5, (int) ngrams.size());
    
}

void ngramBuilderTest::testNgramHash()
{
    Ngram n1;    
    n1.tokens[0] = "lazy";
    n1.tokens[1] = "fox";
    n1.len = 2;
    
    Ngram n2;
    n2.tokens[0] = "lazy";
    n2.tokens[1] = "fox";
    n2.tokens[2] = "jumped";
    n2.len = 3;
    
    CPPUNIT_ASSERT(n1.hash() > 0);
    
    CPPUNIT_ASSERT(n1.hash() != n2.hash());
}



bool ngramBuilderTest::ngramMatcher(Ngram& expected, Ngram& actual)
{
    return expected.equals(actual);
}

bool ngramBuilderTest::ngramSimpleMatcher(NgramSimple & expected, NgramSimple const& actual)
{
    return expected.equals(actual);
}



