/*
 * File:   networkTest.h
 * Author: tlibal
 *
 * Created on Dec 29, 2015, 4:41:05 PM
 */

#ifndef NETWORKTEST_H
#define	NETWORKTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../src/Network.h"

class networkTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(networkTest);

    CPPUNIT_TEST(testPortNoSetAndGet);
    CPPUNIT_TEST(testHostnameSetAndGet);
    CPPUNIT_TEST(testFileLoggerSetAndGet);

    CPPUNIT_TEST_SUITE_END();

public:
    networkTest();
    virtual ~networkTest();
    void setUp();
    void tearDown();

private:
    Network* network;
    void testPortNoSetAndGet();
    void testHostnameSetAndGet();
    void testFileLoggerSetAndGet();
};

#endif	/* NETWORKTEST_H */

