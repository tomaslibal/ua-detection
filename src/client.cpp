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
 * Depending on the command, additional arguments may be passed. The command
 * and the additional arguments are space separated. 
 *
 * However, this client has its own set of arguments to be passed in. The only
 * mandatory one is the string to be passed to the server. 
 * 
 * For the classification of a user-agent string the following will return 
 * the list of classes for the given user-agent:
 * 
 *     argv[1] "eval Mozilla/5.0 (Android En) Firefox/44.0"
 * 
 * For more info, look into the Client::process_arguments method.
 */
int main(int argc, char** argv) {
    
    /*
     * 
     */
    Client mainClient;
       
    /*
     * Read the user input which should include the command and the user-agent 
     * string to be passed to the server.
     */
    mainClient.process_arguments(argv, argc);
    
    /*
     * Connects to the server, sends the request and waits for a response 
     * and then prints the response.
     */
    mainClient.start();
        
    return 0;
}

