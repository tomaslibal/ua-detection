/*
 * File:   programConfigTest.h
 * Author: tlibal
 *
 * Created on Dec 28, 2015, 12:16:05 PM
 */

#ifndef PROGRAMCONFIGTEST_H
#define	PROGRAMCONFIGTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../src/ProgramConfig.h"

class programConfigTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(programConfigTest);

    CPPUNIT_TEST(testProgramConfigObjectWithParams);
    
    CPPUNIT_TEST_SUITE_END();

public:
    programConfigTest();
    virtual ~programConfigTest();
    void setUp();
    void tearDown();

private: 
    ProgramConfig* pconf;   
    void testProgramConfigObjectWithParams();
};

#endif	/* PROGRAMCONFIGTEST_H */

