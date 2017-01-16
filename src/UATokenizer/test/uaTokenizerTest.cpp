/*
 * File:   uaTokenizerTest.cpp
 * Author: tlibal
 *
 * Created on Dec 28, 2015, 12:45:11 PM
 */

#include "uaTokenizerTest.h"

#include <string>
#include <vector>

#include <iostream>
#include <ctime>

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
    std::vector<std::string> tokens;
    std::string sentence = "Subtracting equation (53) from equation (52) gives 8";
    tok->tokenize(sentence, &tokens);
    
    CPPUNIT_ASSERT_MESSAGE("Tokens.size()=8", 8 == tokens.size());
    CPPUNIT_ASSERT_MESSAGE("Tokens[0]=Subtracting", "Subtracting" == tokens[0]);
    CPPUNIT_ASSERT_MESSAGE("Tokens[1]=equation", "equation" == tokens[1]);
    CPPUNIT_ASSERT_MESSAGE("Tokens[2]=53", "53" == tokens[2]);
    CPPUNIT_ASSERT_MESSAGE("Tokens[3]=from", "from" == tokens[3]);
    CPPUNIT_ASSERT_MESSAGE("Tokens[4]=equation", "equation" == tokens[4]);
    CPPUNIT_ASSERT_MESSAGE("Tokens[5]=52", "52" == tokens[5]);
    CPPUNIT_ASSERT_MESSAGE("Tokens[6]=gives", "gives" == tokens[6]);
    CPPUNIT_ASSERT_MESSAGE("Tokens[7]=8", "8" == tokens[7]);
}

void uaTokenizerTest::testStaticTokenizeSkipsLeadingSeparatorsOrWhitespace() {
    std::vector<std::string> tokens;
    std::string sentence = "  (complementary function of the differential equation";
    tok->tokenize(sentence, &tokens);
    
    CPPUNIT_ASSERT_MESSAGE("Tokens.size()=6", 6 == tokens.size());
    CPPUNIT_ASSERT_MESSAGE("Tokens[0]=complementary", "complementary" == tokens[0]);
}

void uaTokenizerTest::testStaticTokenizeBreaksUpUserAgentIntoTokens()
{
    std::vector<std::string> tokens;
    std::string sentence = "Mozilla/5.0 (X11; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0";
    tok->tokenize(sentence, &tokens);
    
    CPPUNIT_ASSERT_MESSAGE("expected Tokens.size()=7, actual=" + std::to_string(tokens.size()), 8 == tokens.size());
    CPPUNIT_ASSERT_MESSAGE("", "Mozilla/5.0" == tokens[0]);
    CPPUNIT_ASSERT_MESSAGE("", "X11" == tokens[1]);
    CPPUNIT_ASSERT_MESSAGE("", "Linux" == tokens[2]);
    CPPUNIT_ASSERT_MESSAGE("", "x86_64" == tokens[3]);
    CPPUNIT_ASSERT_MESSAGE("", "rv" == tokens[4]);
    CPPUNIT_ASSERT_MESSAGE("", "44.0" == tokens[5]);
    CPPUNIT_ASSERT_MESSAGE("", "Gecko/20100101" == tokens[6]);
    CPPUNIT_ASSERT_MESSAGE("", "Firefox/44.0" == tokens[7]);
}

void uaTokenizerTest::testStaticTokenizeForShortUserAgentString()
{
    std::vector<std::string> tokens;
    std::string sentence = "Java/1.7.0_71";
    tok->tokenize(sentence, &tokens);
    
    CPPUNIT_ASSERT_MESSAGE("expected Tokens.size()=1, actual=" + std::to_string(tokens.size()), 1 == tokens.size());
    CPPUNIT_ASSERT_MESSAGE("", "Java/1.7.0_71" == tokens[0]);
}

void uaTokenizerTest::testSetAndGetUserAgentString()
{
    std::string uas = "Mozilla/5.0 Test";
    
    CPPUNIT_ASSERT_MESSAGE("uas is null on init", "" == tok->get_uas());
    
    tok->set_uas(uas);
    
    CPPUNIT_ASSERT_MESSAGE("uas has been set", uas == tok->get_uas());
}

void uaTokenizerTest::testSpeedAndPrintMetrics()
{
    std::string uas = "Mozilla/5.0 (Linux; Android 4.4.4; SM-J110H Build/XYZ) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.1000.10 Mobile Safari/537.36";
    std::vector<std::string> tokens;
    
    std::clock_t t = std::clock();
    int max = 100000;
    for (int i = 0; i < max; i++) {
        tok->tokenize(uas, &tokens);
    }
    t = std::clock() - t;
    
    double elapsed = ((double) t) / CLOCKS_PER_SEC;
    long total_chr = uas.length() * max;
    long total_s = total_chr * sizeof(char);
    std::cout << std::endl << max << " iterations, t=" << elapsed << "s" << std::endl;
    std::cout << "avg " << (total_s / elapsed / 1024) << " kB/s" << std::endl;
}



