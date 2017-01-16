/*
 * File:   naiveBayesClassifierTest.h
 * Author: tlibal
 *
 * Created on Dec 29, 2015, 4:41:05 PM
 */

#ifndef NAIVEBAYESCLASSIFIERTEST_H
#define	NAIVEBAYESCLASSIFIERTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../src/NaiveBayessClassifier.h"

class naiveBayesClassifierTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(naiveBayesClassifierTest);

    CPPUNIT_TEST(testIncPriorsFreqIncreasesFrequencyByOne);
    CPPUNIT_TEST(testIncProbForNgram);
    CPPUNIT_TEST(testNgramPriorGoesUpIfMoreFrequent);

    CPPUNIT_TEST_SUITE_END();

public:
    naiveBayesClassifierTest();
    virtual ~naiveBayesClassifierTest();
    void setUp();
    void tearDown();

private:
    NaiveBayessClassifier* nbc;
    void testIncPriorsFreqIncreasesFrequencyByOne();
    void testIncProbForNgram();
    void testNgramPriorGoesUpIfMoreFrequent();
};

#endif	/* NAIVEBAYESCLASSIFIERTEST_H */

