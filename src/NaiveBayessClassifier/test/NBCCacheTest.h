/*
 * File:   NBCCacheTest.h
 * Author: tlibal
 *
 * Created on Dec 29, 2015, 4:41:05 PM
 */

#ifndef NBCCACHETEST_H
#define	NBCCACHETEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../src/NBCCache.h"

class NBCCacheTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(NBCCacheTest);

    CPPUNIT_TEST(testMissesIntCacheIfNotPreset);
    CPPUNIT_TEST(testMissesDoubleCacheIfNotPreset);
    CPPUNIT_TEST(testHitsIntCacheIfPresent);
    CPPUNIT_TEST(testHitsDoubleCacheIfPresent);
    CPPUNIT_TEST(testMissesIntCacheIfPresentButStale);
    CPPUNIT_TEST(testHitsIntCacheIfPresentAndNotStale);

    CPPUNIT_TEST_SUITE_END();

public:
    NBCCacheTest();
    virtual ~NBCCacheTest();
    void setUp();
    void tearDown();

private:
    NBCCache* nbcc;
    void testMissesIntCacheIfNotPreset();
    void testMissesDoubleCacheIfNotPreset();
    void testHitsIntCacheIfPresent();
    void testHitsDoubleCacheIfPresent();
    void testMissesIntCacheIfPresentButStale();
    void testHitsIntCacheIfPresentAndNotStale();
};

#endif	/* NBCCACHETEST_H */

