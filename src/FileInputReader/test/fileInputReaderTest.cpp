/*
 * File:   fileInputReaderTest.cpp
 * Author: tlibal
 *
 * Created on Dec 28, 2015, 12:16:06 PM
 */

#include "fileInputReaderTest.h"

#include <functional>
#include <string>
#include <vector>

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
    
    std::function<void (std::string const&)> test_callback = [&numLinesRead](std::string const& line) {
        numLinesRead++;
    };
    
    reader->readLines("../../FileInputReader/test/resources/test.txt", test_callback);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE("The file has 3 lines thus 3 lines should have been read", 3, numLinesRead);
}

void fileInputReaderTest::testReadLinesPassesInTheLinesAsStrings() {
    std::vector<std::string> lines;
    
    std::function<void (std::string const&)> test_callback = [&lines](std::string const& line) {
        lines.push_back(line);
    };
    
    reader->readLines("../../FileInputReader/test/resources/test.txt", test_callback);
    
    for(unsigned int i = 0; i < lines.size(); i++) {
        if (i == 0) {
            CPPUNIT_ASSERT_MESSAGE("The first line should read 'foo,1'", "foo,1" == lines[i]);
        }
        if (i == 1) {
            CPPUNIT_ASSERT_MESSAGE("The second line should read 'bar,2'", "bar,2" == lines[i]);
        }
        if (i == 2) {
            CPPUNIT_ASSERT_MESSAGE("The third line should read 'baz,3'", "baz,3" == lines[i]);           
        }
    }
    
}

void fileInputReaderTest::testReadLinesWontExecuteTheCallbackIfFileNotFound() {
    std::string nonExistingFile = "bar.txt";
    
    std::function<void (std::string const&)> test_callback = [](std::string const& line) {        
        CPPUNIT_FAIL("The callback should not be invoked when the file cannot be found");
    };
    
    reader->readLines(nonExistingFile, test_callback);
}
