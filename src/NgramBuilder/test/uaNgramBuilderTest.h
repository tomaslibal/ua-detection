/*
 * File:   uaNgramBuilderTest.h
 * Author: tlibal
 *
 * Created on Dec 28, 2015, 4:14:15 PM
 */

#ifndef UANGRAMBUILDERTEST_H
#define	UANGRAMBUILDERTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../src/UaNgramBuilder.h"

class uaNgramBuilderTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(uaNgramBuilderTest);

    CPPUNIT_TEST(testFromTokenListBuildsNgrams);
    CPPUNIT_TEST(testLevelDefaultsToThree);
    CPPUNIT_TEST(testSetAndGetLevel);
    CPPUNIT_TEST(testCannotSetNegativeLevel);

    CPPUNIT_TEST_SUITE_END();

public:
    uaNgramBuilderTest();
    virtual ~uaNgramBuilderTest();
    void setUp();
    void tearDown();

private:
    UaNgramBuilder* uaNgramBuilder;
    void testLevelDefaultsToThree();
    void testSetAndGetLevel();
    void testCannotSetNegativeLevel();
    void testFromTokenListBuildsNgrams();
    
    bool ngramSimpleMatcher(NgramSimple& expected, const NgramSimple& actual);
};

#endif	/* UANGRAMBUILDERTEST_H */

