/*
 * File:   fileInputReaderTest.cpp
 * Author: tlibal
 *
 * Created on Dec 28, 2015, 12:16:06 PM
 */

#include "fileInputReaderTest.h"


CPPUNIT_TEST_SUITE_REGISTRATION(fileInputReaderTest);

fileInputReaderTest::fileInputReaderTest() {
}

fileInputReaderTest::~fileInputReaderTest() {
}

void fileInputReaderTest::setUp() {
    reader = new FileInputReader();
}

void fileInputReaderTest::tearDown() {
    delete reader;
}

void fileInputReaderTest::testReadLines() {
    
}

