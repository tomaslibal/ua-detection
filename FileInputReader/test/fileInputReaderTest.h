/*
 * File:   fileInputReaderTest.h
 * Author: tlibal
 *
 * Created on Dec 28, 2015, 12:16:05 PM
 */

#ifndef FILEINPUTREADERTEST_H
#define	FILEINPUTREADERTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../src/FileInputReader.h"

class fileInputReaderTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(fileInputReaderTest);

    CPPUNIT_TEST(testReadLines);

    CPPUNIT_TEST_SUITE_END();

public:
    fileInputReaderTest();
    virtual ~fileInputReaderTest();
    void setUp();
    void tearDown();

private:
    FileInputReader* reader;
    void testReadLines();
};

#endif	/* FILEINPUTREADERTEST_H */

