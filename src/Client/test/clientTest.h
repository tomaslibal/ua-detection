/*
 * File:   uaNgramBuilderTest.h
 * Author: tlibal
 *
 * Created on Dec 28, 2015, 4:14:15 PM
 */

#ifndef CLIENTTEST_H
#define CLIENTTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../src/Client.h"

class clientTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(clientTest);

    CPPUNIT_TEST(test);

    CPPUNIT_TEST_SUITE_END();

public:
    clientTest();
    virtual ~clientTest();
    void setUp();
    void tearDown();

private:
    Client client;
    void test();
};

#endif  /* CLIENTTEST_H */

