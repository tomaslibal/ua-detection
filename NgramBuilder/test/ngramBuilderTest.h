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

    CPPUNIT_TEST_SUITE_END();

public:
    ngramBuilderTest();
    virtual ~ngramBuilderTest();
    void setUp();
    void tearDown();

private:
    NgramBuilder* ngBuilder;
    void testFromTokenListBuildsNgrams();
};

#endif	/* NGRAMBUILDERTEST_H */

