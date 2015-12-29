/*
 * File:   naiveBayesClassifierTest.cpp
 * Author: tlibal
 *
 * Created on Dec 29, 2015, 4:41:06 PM
 */

#include "naiveBayesClassifierTest.h"


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
    
}

