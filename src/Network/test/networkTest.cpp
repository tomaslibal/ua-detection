/*
 * File:   networkTest.cpp
 * Author: tlibal
 *
 * Created on Dec 29, 2015, 4:41:06 PM
 */

#include "networkTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(networkTest);

networkTest::networkTest() {
}

networkTest::~networkTest() {
}

void networkTest::setUp() {
    network = new Network();
}

void networkTest::tearDown() {
    delete network;
}

void networkTest::testPortNoSetAndGet()
{
  network->set_port_no(42);
  
  CPPUNIT_ASSERT(network->get_port_no() == 42);
}


