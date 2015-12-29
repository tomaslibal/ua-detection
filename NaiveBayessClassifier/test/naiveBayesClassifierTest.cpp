/*
 * File:   naiveBayesClassifierTest.cpp
 * Author: tlibal
 *
 * Created on Dec 29, 2015, 4:41:06 PM
 */

#include "naiveBayesClassifierTest.h"

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

