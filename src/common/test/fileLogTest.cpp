/*
 * File:   fileLogTest.cpp
 * Author: tlibal
 *
 * Created on Feb 5, 2016 
 */

#include "fileLogTest.h"
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION(fileLogTest);

fileLogTest::fileLogTest()
{
}

fileLogTest::~fileLogTest()
{
}

void fileLogTest::setUp()
{
    logger = new FileLog();
}

void fileLogTest::tearDown()
{
    delete logger;
}

void fileLogTest::testFoo()
{
}
