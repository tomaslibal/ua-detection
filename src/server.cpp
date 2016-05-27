/* 
 * File:   server.cpp
 * Author: tlibal
 *
 * Created on January 5, 2016, 8:08 PM
 */
 
 // Todo - add a flag which will not start the background service, like --no-tcp

#include <cstdlib>
#include <iostream>
#include <csignal>

#include "common/src/FileLog.h"
#include "Server/src/Server.h"

/*
 * 
 */
int main(int argc, char** argv) {
    
    /*
     * Server module
     */
    Server server;
    
    FileLog logger;
    
    server.set_logger(logger);
    
    logger.log("Initializing Server");

    
    /*
     * catch the ctrl+c interrupt signal
     */
    signal(SIGINT, [] (int signum) {        
        if (Server::sockfd >= 0) {
            close(Server::sockfd);
        }
        exit(signum);
    });
    
    server.learn();
    
    server.start();
    
    return 0;
}

