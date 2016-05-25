/* 
 * File:   client.cpp
 * Author: tlibal
 *
 * Created on January 4, 2016, 8:51 PM
 */

#include <cstdlib>
#include <iostream>
#include <string>

#include "common/src/FileLog.h"
#include "common/src/uadet2.h"
#include "Client/src/Client.h"

/*
 *
 */
int main(int argc, char** argv) {
    
    /*
     * 
     */
    Client mainClient;
       
    /*
     * Initialize the file logger by setting the output log path to the current working directory
     * 
     */
    FileLog logger;  
    
    /*
     * Set the log file to be the client's log 
     */
    logger.setPath("./uadet2.client.log.txt");
    
    /*
     * Read user input
     */
    logger.log("Processing program arguments");

    logger.log("Got " + std::to_string(argc) + " arguments");
    
    mainClient.process_arguments(argv, argc);
    
    mainClient.start();
        
    return 0;
}

