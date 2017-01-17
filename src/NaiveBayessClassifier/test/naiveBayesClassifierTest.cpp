/*
 * File:   naiveBayesClassifierTest.cpp
 * Author: tlibal
 *
 * Created on Dec 29, 2015, 4:41:06 PM
 */

#include "naiveBayesClassifierTest.h"

#include <iostream>

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(naiveBayesClassifierTest);

naiveBayesClassifierTest::naiveBayesClassifierTest() {
}

naiveBayesClassifierTest::~naiveBayesClassifierTest() {
}

void naiveBayesClassifierTest::setUp() {
    nbc = new NaiveBayessClassifier();
}

void naiveBayesClassifierTest::tearDown() {
    delete nbc;
}

void naiveBayesClassifierTest::testIncPriorsFreqIncreasesFrequencyByOne() {
    string cat = "smart_tv";
    double p = nbc->prob_category(cat);
    
    CPPUNIT_ASSERT_MESSAGE("Probability of an unknown category is 0", p == 0.0);
    
    string test_data = "foo bar";
    nbc->add_data(test_data, cat);
    
    p = nbc->prob_category(cat);
    
    CPPUNIT_ASSERT_MESSAGE("Probability should be 1/1 = 1", p == 1.0);
    
    string cat2 = "mobile";
    nbc->add_data(test_data, cat2);
    
    p = nbc->prob_category(cat2);
    
    CPPUNIT_ASSERT_MESSAGE("Probability should be 1/2 = 1", p == 0.5);
}

void naiveBayesClassifierTest::testIncProbForNgram()
{
    Ngram n1;
    n1.tokens[0] = "lazy";
    n1.tokens[1] = "red";
    n1.tokens[2] = "jumps";
    n1.len = 3;    
    
    nbc->add_data("lazy red pelican", "foo");
    
    double p = nbc->prob_ngram(n1);
    
    nbc->add_data("jumpy jerry jumps up", "bar");
    
    double p2 = nbc->prob_ngram(n1);
    
    CPPUNIT_ASSERT_MESSAGE("probability should have decreased", p2 < p);
}

void naiveBayesClassifierTest::testNgramPriorGoesUpIfMoreFrequent()
{
    Ngram n1;
    n1.tokens[0] = "lazy";
    n1.tokens[1] = "red";
    n1.tokens[2] = "jumps";
    n1.len = 3;
    
    nbc->add_data("speedy spike", "foo");
    
    double p = nbc->prob_ngram(n1);
    
    nbc->add_data("lazy red jumps over and over", "bar");
    
    double p2 = nbc->prob_ngram(n1);
    
    CPPUNIT_ASSERT_MESSAGE("probability should have increased", p2 > p);
}



