/*
 * File:   networkTest.cpp
 * Author: tlibal
 *
 * Created on Dec 29, 2015, 4:41:06 PM
 */

#include "networkTest.h"

#include "../../common/src/FileLog.h"

CPPUNIT_TEST_SUITE_REGISTRATION(networkTest);

networkTest::networkTest() {
}

networkTest::~networkTest() {
}

void networkTest::setUp() {
    NetworkConfig networkConfig(10128, "localhost");
    network = new Network(networkConfig);
}

void networkTest::tearDown() {
    delete network;
}

void networkTest::testPortNoSetAndGet()
{
    NetworkConfig networkConfig(42, "localhost");
    Network n(networkConfig);
  
    CPPUNIT_ASSERT_MESSAGE(std::to_string(n.get_port_no()), n.get_port_no() == 42);
}

void networkTest::testHostnameSetAndGet()
{
    NetworkConfig networkConfig(42, "example.com");
    Network n(networkConfig);
  
    CPPUNIT_ASSERT_MESSAGE(n.get_hostname(), n.get_hostname().compare("example.com") == 0);
}


void networkTest::testFileLoggerSetAndGet()
{
    CPPUNIT_ASSERT(network->get_file_log() == nullptr);
  
    FileLog fileLog;
  
    network->set_file_log(&fileLog);
  
    CPPUNIT_ASSERT(network->get_file_log() == &fileLog);
}


