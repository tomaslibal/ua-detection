/*
 * File:   fileLogTest.h
 * Author: tlibal
 *
 * Created on Feb 5, 2016
 */

#ifndef FILELOGTEST_H
#define	FILELOGTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../src/FileLog.h"

class fileLogTest: public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(fileLogTest);

    CPPUNIT_TEST(testFoo);
    
    CPPUNIT_TEST_SUITE_END();

public:
    fileLogTest();
    virtual ~fileLogTest();
    void setUp();
    void tearDown();

private:
    FileLog* logger;
    void testFoo();
};

#endif	/* FILELOGTEST_H */

