/*
 * File:   naiveBayesClassifierTest.cpp
 * Author: tlibal
 *
 * Created on Dec 29, 2015, 4:41:06 PM
 */

#include "NBCCacheTest.h"
#include <ctime>

CPPUNIT_TEST_SUITE_REGISTRATION(NBCCacheTest);

NBCCacheTest::NBCCacheTest()
{
}

NBCCacheTest::~NBCCacheTest()
{
}

void NBCCacheTest::setUp()
{
    CppUnit::TestFixture::setUp();
    nbcc = new NBCCache();
}

void NBCCacheTest::tearDown()
{
    CppUnit::TestFixture::tearDown();
    delete nbcc;
}



void NBCCacheTest::testMissesIntCacheIfNotPreset()
{
    bool result = nbcc->in_int_cache("foo");
    
    CPPUNIT_ASSERT_EQUAL(false, result);
}

void NBCCacheTest::testMissesDoubleCacheIfNotPreset()
{
    bool result = nbcc->in_dbl_cache("foo");
    
    CPPUNIT_ASSERT_EQUAL(false, result);
}

void NBCCacheTest::testHitsIntCacheIfPresent()
{
    nbcc->insert_int_cache("foo", 42);
    
    int result = nbcc->get_int_cache("foo");
    
    CPPUNIT_ASSERT_EQUAL(42, result);
}

void NBCCacheTest::testHitsDoubleCacheIfPresent()
{
    nbcc->insert_dbl_cache("foo", 42.0);
    
    double result = nbcc->get_dbl_cache("foo");
    
    CPPUNIT_ASSERT_EQUAL(42.0, result);
}

void NBCCacheTest::testMissesIntCacheIfPresentButStale()
{
    unsigned int now = (unsigned int) std::time(nullptr);
    nbcc->insert_int_cache("foo", 42);
    nbcc->timestamps["intfoo"] = now - 3601; // make the item expired
    
    bool result = nbcc->in_int_cache("foo");
    
    CPPUNIT_ASSERT_EQUAL(false, result);
}

void NBCCacheTest::testHitsIntCacheIfPresentAndNotStale()
{
    unsigned int now = (unsigned int) std::time(nullptr);
    nbcc->insert_int_cache("foo", 42);
    nbcc->timestamps["intfoo"] = now - 3599; // item's validity ok
    
    bool result = nbcc->in_int_cache("foo");
    
    CPPUNIT_ASSERT_EQUAL(true, result);
}

void NBCCacheTest::testUpdatingIntCacheRefreshesTheValidity()
{
unsigned int now = (unsigned int) std::time(nullptr);
    nbcc->insert_int_cache("foo", 42);
    nbcc->timestamps["intfoo"] = now - 3601; // make the item expired
    
    nbcc->insert_int_cache("foo", 43);
    
    bool result = nbcc->in_int_cache("foo");
    
    CPPUNIT_ASSERT_EQUAL(true, result);
}



