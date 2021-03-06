/*
 * File:   uaTokenizerTest.h
 * Author: tlibal
 *
 * Created on Dec 28, 2015, 12:45:11 PM
 */

#ifndef UATOKENIZERTEST_H
#define	UATOKENIZERTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../src/UATokenizer.h"

class uaTokenizerTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(uaTokenizerTest);

    CPPUNIT_TEST(testStaticTokenizeBreaksUpSentenceIntoTokens);
    CPPUNIT_TEST(testStaticTokenizeSkipsLeadingSeparatorsOrWhitespace);
    CPPUNIT_TEST(testStaticTokenizeBreaksUpUserAgentIntoTokens);
    CPPUNIT_TEST(testStaticTokenizeForShortUserAgentString);
    CPPUNIT_TEST(testSpeedAndPrintMetrics);

    CPPUNIT_TEST_SUITE_END();

public:
    uaTokenizerTest();
    virtual ~uaTokenizerTest();
    void setUp();
    void tearDown();

private:
    UATokenizer* tok;
    void testStaticTokenizeBreaksUpSentenceIntoTokens();
    void testStaticTokenizeSkipsLeadingSeparatorsOrWhitespace();
    void testStaticTokenizeBreaksUpUserAgentIntoTokens();
    void testStaticTokenizeForShortUserAgentString();    
    
    void testSpeedAndPrintMetrics();
};

#endif	/* UATOKENIZERTEST_H */

