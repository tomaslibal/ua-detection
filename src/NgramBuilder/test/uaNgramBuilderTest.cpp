/*
 * File:   uaNgramBuilderTest.cpp
 * Author: tlibal
 *
 * Created on Dec 28, 2015, 4:14:15 PM
 */

#include "uaNgramBuilderTest.h"


CPPUNIT_TEST_SUITE_REGISTRATION(uaNgramBuilderTest);

uaNgramBuilderTest::uaNgramBuilderTest() {
}

uaNgramBuilderTest::~uaNgramBuilderTest() {
}

void uaNgramBuilderTest::setUp() {
    uaNgramBuilder = new UaNgramBuilder();
}

void uaNgramBuilderTest::tearDown() {
    delete uaNgramBuilder;
}

void uaNgramBuilderTest::testLevelDefaultsToThree()
{
    CPPUNIT_ASSERT(uaNgramBuilder->get_level() == 3);
}

void uaNgramBuilderTest::testSetAndGetLevel()
{
    uaNgramBuilder->set_level(5);
    
    CPPUNIT_ASSERT(uaNgramBuilder->get_level() == 5);
    
    uaNgramBuilder->set_level(1);
    
    CPPUNIT_ASSERT(uaNgramBuilder->get_level() == 1);
}

void uaNgramBuilderTest::testCannotSetNegativeLevel()
{
    uaNgramBuilder->set_level(-1);
    
    CPPUNIT_ASSERT(uaNgramBuilder->get_level() == 3);
}


void uaNgramBuilderTest::testFromTokenListBuildsNgrams() 
{
    std::vector<NgramSimple> ngrams;
    std::string sentence = "Mozilla/5.0 (X11; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0";
    
    uaNgramBuilder->fromUserAgentString(sentence, &ngrams);
    
    /*
     * There are 7 tokens, so 5 ngrams of length 3 can be created. The last two tokens don't have enough tokens ahead to be able to form a 3-gram.
     */
    CPPUNIT_ASSERT(ngrams.size() == 5);
    
    NgramSimple exp1;
    exp1.len = 3;
    exp1.sentence = "Mozilla/5.0 (X11; Linux";
    exp1.starts[0] = 0; exp1.starts[1] = 12; exp1.starts[2] = 18;
    exp1.lens[0] = 11; exp1.lens[1] = 5; exp1.lens[2] = 5;
    
    CPPUNIT_ASSERT(ngramSimpleMatcher(exp1, ngrams[0]) == true);
    
    NgramSimple exp2;
    exp2.len = 3;
    exp2.sentence = "(X11; Linux x86_64;";
    exp2.starts[0] = 0; exp2.starts[1] = 6; exp2.starts[2] = 12;
    exp2.lens[0] = 5; exp2.lens[1] = 5; exp2.lens[2] = 7;
    
    CPPUNIT_ASSERT(ngramSimpleMatcher(exp2, ngrams[1]) == true);
    
    NgramSimple exp3;
    exp3.len = 3;
    exp3.sentence = "Linux x86_64; rv:44.0)";
    exp3.starts[0] = 0; exp3.starts[1] = 6; exp3.starts[2] = 14;
    exp3.lens[0] = 5; exp3.lens[1] = 7; exp3.lens[2] = 8;
    
    CPPUNIT_ASSERT(ngramSimpleMatcher(exp3, ngrams[2]) == true);
    
    NgramSimple exp4;
    exp4.len = 3;
    exp4.sentence = "x86_64; rv:44.0) Gecko/20100101";
    exp4.starts[0] = 0; exp4.starts[1] = 8; exp4.starts[2] = 17;
    exp4.lens[0] = 7; exp4.lens[1] = 8; exp4.lens[2] = 14;
    
    CPPUNIT_ASSERT(ngramSimpleMatcher(exp4, ngrams[3]) == true);
    
    NgramSimple exp5;
    exp5.len = 3;
    exp5.sentence = "rv:44.0) Gecko/20100101 Firefox/44.0";
    exp5.starts[0] = 0; exp5.starts[1] = 9; exp5.starts[2] = 24;
    exp5.lens[0] = 8; exp5.lens[1] = 14; exp5.lens[2] = 12;
    
    CPPUNIT_ASSERT(ngramSimpleMatcher(exp5, ngrams[4]) == true);
}

bool uaNgramBuilderTest::ngramSimpleMatcher(NgramSimple & expected, NgramSimple const& actual)
{
    return expected.equals(actual);
}


