/*
 * File:   programConfigTest.cpp
 * Author: tlibal
 *
 * Created on Dec 28, 2015, 12:16:06 PM
 */

#include "programConfigTest.h"
#include <iostream>
CPPUNIT_TEST_SUITE_REGISTRATION(programConfigTest);

programConfigTest::programConfigTest() {
}

programConfigTest::~programConfigTest() {
}

void programConfigTest::setUp() {
    pconf = new ProgramConfig();    
}

void programConfigTest::tearDown() {
    delete pconf;
}

void programConfigTest::testProgramConfigObjectWithParams() {
    ProgramConfigObject confObj(0, "foo");

    pconf->path = "../../common/config/server.txt";
    pconf->update(confObj);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("port no. is set to 10128", 10128, confObj.portno);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("hostname is set to localhost", std::string("localhost"), confObj.hostname);
}
