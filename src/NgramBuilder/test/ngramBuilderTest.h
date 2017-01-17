/*
 * File:   ngramBuilderTest.h
 * Author: tlibal
 *
 * Created on Dec 28, 2015, 4:14:15 PM
 */

#ifndef NGRAMBUILDERTEST_H
#define	NGRAMBUILDERTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../src/NgramBuilder.h"

class ngramBuilderTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(ngramBuilderTest);

    CPPUNIT_TEST(testFromTokenListBuildsNgrams);
    CPPUNIT_TEST(testLevelDefaultsToThree);
    CPPUNIT_TEST(testSetAndGetLevel);
    CPPUNIT_TEST(testCannotSetNegativeLevel);
    CPPUNIT_TEST(testSetDynamicFlag);
    CPPUNIT_TEST(testZeroLevelMeansMaxPossibleNgram);
    CPPUNIT_TEST(testNgramHash);
    CPPUNIT_TEST(testNgramSimple);
    CPPUNIT_TEST(testNgramSimpleToString);
    CPPUNIT_TEST(testNgramVsNgramSimplePerf);
    CPPUNIT_TEST(testNgramVsNgramSimplePerfToString);

    CPPUNIT_TEST_SUITE_END();

public:
    ngramBuilderTest();
    virtual ~ngramBuilderTest();
    void setUp();
    void tearDown();

private:
    NgramBuilder* ngBuilder;
    void testLevelDefaultsToThree();
    void testSetAndGetLevel();
    void testCannotSetNegativeLevel();
    void testFromTokenListBuildsNgrams();
    void testSetDynamicFlag();
    void testZeroLevelMeansMaxPossibleNgram();
    void testNgramHash();
    void testNgramSimple();
    void testNgramSimpleToString();
    void testNgramVsNgramSimplePerf();
    void testNgramVsNgramSimplePerfToString();
    
    bool ngramMatcher(Ngram& expected, Ngram& actual);
    bool ngramSimpleMatcher(NgramSimple& expected, const NgramSimple& actual);
};

#endif	/* NGRAMBUILDERTEST_H */

