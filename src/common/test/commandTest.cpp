/*
 * File:   commandTest.cpp
 * Author: tlibal
 *
 * Created on Jul 1, 2016
 */

#include "commandTest.h"

#include <iostream>
#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION(commandTest);

commandTest::commandTest()
{
}

commandTest::~commandTest()
{
}

void commandTest::setUp()
{
    
}

void commandTest::tearDown()
{
    
}

void commandTest::testBuilder()
{    
    Command cmd = Command::Builder()
        .withCommandSource(CommandSource::Client)
        .withName("eval")
        .withData("Mozilla/5.0 (Linux; Android 6; en) Firefox/45.0")
        .build();
        
    CPPUNIT_ASSERT(cmd.getCommandSource() == CommandSource::Client);
    CPPUNIT_ASSERT(cmd.getName().compare("eval") == 0);
    CPPUNIT_ASSERT(cmd.getData().compare("Mozilla/5.0 (Linux; Android 6; en) Firefox/45.0") == 0);
}

void commandTest::testToStringClient()
{
    std::string expectedString = "client:eval Mozilla/5.0 (Linux; Android 6; en) Firefox/45.0";
    
    Command cmd = Command::Builder()
        .withCommandSource(CommandSource::Client)
        .withName("eval")
        .withData("Mozilla/5.0 (Linux; Android 6; en) Firefox/45.0")
        .build();
        
    CPPUNIT_ASSERT(cmd.toString().compare(expectedString) == 0);
}

void commandTest::testToStringServer()
{
    std::string expectedString = "server:response class1:0.99 class2:0.91 class3:0.01";
    
    Command cmd = Command::Builder()
        .withCommandSource(CommandSource::Server)
        .withName("response")
        .withData("class1:0.99 class2:0.91 class3:0.01")
        .build();
        
    CPPUNIT_ASSERT(cmd.toString().compare(expectedString) == 0);
}



