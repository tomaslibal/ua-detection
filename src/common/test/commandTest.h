/*
 * File:   commandTest.h
 * Author: tlibal
 *
 * Created on Feb 5, 2016
 */

#ifndef COMMANDTEST_H
#define	COMMANDTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../src/Command.h"

class commandTest: public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(commandTest);

    CPPUNIT_TEST(testBuilder);
    CPPUNIT_TEST(testToStringClient);
    CPPUNIT_TEST(testToStringServer);
    
    CPPUNIT_TEST_SUITE_END();

public:
    commandTest();
    virtual ~commandTest();
    void setUp();
    void tearDown();

private:
    void testBuilder();
    void testToStringClient();
    void testToStringServer();
};

#endif	/* COMMANDTEST_H */

