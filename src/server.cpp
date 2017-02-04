/* 
 * File:   server.cpp
 * Author: tlibal
 *
 * Created on January 5, 2016, 8:08 PM
 */

#include <csignal>
#include <iostream>

#include "Server/src/Server.h"

static Server* serverModulePtr = nullptr;

/*
 * 
 */
int main(int argc, char** argv) {
    
    /*
     * Server module resource handler
     */
    Server server;
    serverModulePtr = &server;
    
    /*
     * catch the interrupt signal
     */
    signal(SIGINT, [] (int signum) {
        std::cout << "Received the SIGINT, exiting now..." << std::endl;
        if (serverModulePtr != nullptr) {
            serverModulePtr->stop();
            serverModulePtr->~Server();
        }
        exit(signum);
    });
    
    /*
     * Reads the data file and trains the Naive Bayess Classifier
     */
    server.learn();
    
    /*
     * Starts accepting TCP connection and replies with responses to the client requests
     */
    server.start();
    
    return 0;
}

