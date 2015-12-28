/*
 * File:   fileInputReaderTest.cpp
 * Author: tlibal
 *
 * Created on Dec 28, 2015, 12:16:06 PM
 */

#include "fileInputReaderTest.h"

#include <functional>
#include <string>

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

void fileInputReaderTest::testReadLinesReadsAllLinesInTheFile() {
    int numLinesRead = 0;
    
    std::function<void (std::string)> test_callback = [&numLinesRead](std::string line) {
        numLinesRead++;
    };
    
    reader->readLines("FileInputReader/test/resources/test.txt", test_callback);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE("The file has 3 lines thus 3 lines should have been read", 3, numLinesRead);
}

