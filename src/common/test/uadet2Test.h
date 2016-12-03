/*
 * File:   uadet2Test.h
 * Author: tlibal
 *
 * Created on December 3, 2016
 */

#ifndef uadet2Test_H
#define	uadet2Test_H

#include <cppunit/extensions/HelperMacros.h>

#include "../src/uadet2.h"

class uadet2Test: public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(uadet2Test);

    CPPUNIT_TEST(testFoo);
    
    CPPUNIT_TEST_SUITE_END();

public:
    uadet2Test();
    virtual ~uadet2Test();
    void setUp();
    void tearDown();

private:
    void testFoo();
};

#endif	/* uadet2Test_H */

