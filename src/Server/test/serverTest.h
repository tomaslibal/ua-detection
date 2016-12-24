/*
 * File:   serverTest.h
 * Author: tlibal
 *
 * Created on Dec 29, 2015, 4:41:05 PM
 */

#ifndef serverTest_H
#define	serverTest_H

#include <cppunit/extensions/HelperMacros.h>

#include "../src/Server.h"

class serverTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(serverTest);

    CPPUNIT_TEST(testJsonOutput);
    CPPUNIT_TEST(testJsonOutputWithKeyValuePairs);
    CPPUNIT_TEST(testPlaintextOutput);
  
    CPPUNIT_TEST_SUITE_END();

public:
    serverTest();
    virtual ~serverTest();
    void setUp();
    void tearDown();

private:
    Server* svr;
    void testJsonOutput();
    void testJsonOutputWithKeyValuePairs();
    void testPlaintextOutput();
};

#endif	/* serverTest_H */

