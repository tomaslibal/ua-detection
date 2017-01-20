/*
 * File:   ngramBuilderTest.cpp
 * Author: tlibal
 *
 * Created on Dec 28, 2015, 4:14:15 PM
 */

#include "ngramBuilderTest.h"

#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <ctime>

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
    std::vector<NgramSimple> ngrams;
    
    char** tokens = (char**)malloc(sizeof(char*)*4);
    const int num = 4;

    tokens[0] = (char*)malloc(sizeof(char)*12);
    strcpy(tokens[0], "Mozilla/5.0");
    tokens[1] = (char*)malloc(sizeof(char)*4);
    strcpy(tokens[1], "X11");
    tokens[2] = (char*)malloc(sizeof(char)*6);
    strcpy(tokens[2], "Linux");
    tokens[3] = (char*)malloc(sizeof(char)*7);
    strcpy(tokens[3], "x86_64");
    
    ngBuilder->fromTokenList(tokens, num, &ngrams);
    
    CPPUNIT_ASSERT(ngrams.size() == 2);
    
    NgramSimple exp1;
    exp1.len = 3;
    exp1.sentence = "Mozilla/5.0 X11 Linux";
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

void ngramBuilderTest::testNgramSimpleToString()
{
    std::string expected = "Mozilla/5.0 X11 Linux";
    std::string expected2 = "Mozilla/5.0";
    std::string expected3 = "Mozilla/5.0 X11";
    
    std::vector<NgramSimple> ngrams;
    
    char** tokens = (char**)malloc(sizeof(char*)*3);
    const int num = 3;

    tokens[0] = (char*)malloc(sizeof(char)*12);
    strcpy(tokens[0], "Mozilla/5.0");
    tokens[1] = (char*)malloc(sizeof(char)*4);
    strcpy(tokens[1], "X11");
    tokens[2] = (char*)malloc(sizeof(char)*6);
    strcpy(tokens[2], "Linux");
    
    ngBuilder->fromTokenList(tokens, num, &ngrams);
    
    NgramSimple result = ngrams.at(0);
    
    CPPUNIT_ASSERT(expected.compare(result.toString()) == 0);
    
    CPPUNIT_ASSERT(expected2.compare(result.toString(1)) == 0);
    
    CPPUNIT_ASSERT(expected3.compare(result.toString(2)) == 0);
    
    CPPUNIT_ASSERT(expected.compare(result.toString(3)) == 0);
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
    std::vector<NgramSimple> ngrams;
    
    char** tokens = (char**)malloc(sizeof(char*)*5);
    const int num = 5;

    tokens[0] = (char*)malloc(sizeof(char)*12);
    strcpy(tokens[0], "Mozilla/5.0");
    tokens[1] = (char*)malloc(sizeof(char)*4);
    strcpy(tokens[1], "X11");
    tokens[2] = (char*)malloc(sizeof(char)*6);
    strcpy(tokens[2], "Linux");
    tokens[3] = (char*)malloc(sizeof(char)*7);
    strcpy(tokens[3], "x86_64");
    tokens[4] = (char*)malloc(sizeof(char)*13);
    strcpy(tokens[4], "Firefox/60.0");
    
    ngBuilder->set_dynamic(true);
    ngBuilder->fromTokenList(tokens, num, &ngrams);
    
    // There should be 5 ngrams, 1 of length 5, 1 of length 4, 1 of length 3, 1 of length 2 and 1 of length 1
    CPPUNIT_ASSERT_EQUAL(5, (int) ngrams.size());
    
}

void ngramBuilderTest::testNgramHash()
{
    NgramSimple n1;
    n1.sentence = "lazy fox";
    n1.len = 2;
    
    NgramSimple n2;
    n2.sentence = "lazy fox jumped";
    n2.len = 3;
    
    CPPUNIT_ASSERT(n1.hash() > 0);
    
    CPPUNIT_ASSERT(n1.hash() != n2.hash());
}


bool ngramBuilderTest::ngramSimpleMatcher(NgramSimple & expected, NgramSimple const& actual)
{
    return expected.equals(actual);
}



