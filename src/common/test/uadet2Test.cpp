/*
 * File:   uadet2Test.cpp
 * Author: tlibal
 *
 * Created on Feb 5, 2016 
 */

#include "uadet2Test.h"
#include <iostream>
#include <vector>
#include <cmath>

CPPUNIT_TEST_SUITE_REGISTRATION(uadet2Test);

uadet2Test::uadet2Test()
{
}

uadet2Test::~uadet2Test()
{
}

void uadet2Test::setUp()
{
    
}

void uadet2Test::tearDown()
{
    
}

void uadet2Test::test3dimVecLowValues()
{

    std::vector<double> z = { 1.0, 2.0, 3.0 };
    std::vector<double> sm = softmax(z);

    CPPUNIT_ASSERT(compareDouble3decimalPrecision(sm[0], 0.090) == true);
    CPPUNIT_ASSERT(compareDouble3decimalPrecision(sm[1], 0.244) == true);
    CPPUNIT_ASSERT(compareDouble3decimalPrecision(sm[2], 0.665) == true);
}

bool uadet2Test::compareDouble3decimalPrecision(double x, double y)
{
    return trunc(1000. * x) == trunc(1000. * y);
}
