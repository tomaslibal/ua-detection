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
 * This is an example implementation of a client for the uadet2 server. 
 * The server expects a list of arguments, first of which is the command.
 * Depending on the command, additional arguments may be passed. The commands
 * are space separated.
 * 
 * For the classification of a user-agent string the following will return 
 * the list of classes for the given user-agent:
 * 
 *     "eval Mozilla/5.0 (Android En) Firefox/44.0"
 */
int main(int argc, char** argv) {
    
    /*
     * 
     */
    Client mainClient;
       
    /*
     * 
     */
    mainClient.process_arguments(argv, argc);
    
    /*
     * Connects to the server, sends the request and waits for a response and then prints the response
     */
    mainClient.start();
        
    return 0;
}

