/*
 * File:   serverTest.cpp
 * Author: tlibal
 *
 * Created on Dec 29, 2015, 4:41:06 PM
 */

#include "serverTest.h"

#include <vector>
#include <string>


CPPUNIT_TEST_SUITE_REGISTRATION(serverTest);

serverTest::serverTest() {
}

serverTest::~serverTest() {
}

void serverTest::setUp() {
    svr = new Server();
}

void serverTest::tearDown() {
    delete svr;
}

void serverTest::testJsonOutput()
{
   std::vector<std::string> labels;
   std::vector<double> values;

   std::string* actual = svr->json_output(values, labels);

   std::string expected = "{\n}";

   CPPUNIT_ASSERT(actual != nullptr);

   CPPUNIT_ASSERT(expected.compare(*actual) == 0);

   delete actual;
}

void serverTest::testJsonOutputWithKeyValuePairs()
{
    std::vector<std::string> labels = { "foo", "bar" };
    std::vector<double> values = { 42.0, 1337.123 };

    std::string* actual = svr->json_output(values, labels);

    std::string expected = "{\n    \"foo\": 42.000000,\n    \"bar\": 1337.123000\n}";

    CPPUNIT_ASSERT(actual != nullptr);

    CPPUNIT_ASSERT(expected.compare(*actual) == 0);

    delete actual;
}
