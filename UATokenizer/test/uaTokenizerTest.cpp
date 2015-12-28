/*
 * File:   uaTokenizerTest.cpp
 * Author: tlibal
 *
 * Created on Dec 28, 2015, 12:45:11 PM
 */

#include "uaTokenizerTest.h"


CPPUNIT_TEST_SUITE_REGISTRATION(uaTokenizerTest);

uaTokenizerTest::uaTokenizerTest() {
}

uaTokenizerTest::~uaTokenizerTest() {
}

void uaTokenizerTest::setUp() {
    tok = new UATokenizer();
}

void uaTokenizerTest::tearDown() {
    delete tok;
}

void uaTokenizerTest::testStaticTokenizeBreaksUpSentenceIntoTokens() {
    CPPUNIT_ASSERT(true);
}


