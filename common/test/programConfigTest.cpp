/*
 * File:   programConfigTest.cpp
 * Author: tlibal
 *
 * Created on Dec 28, 2015, 12:16:06 PM
 */

#include "programConfigTest.h"

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
    
}
