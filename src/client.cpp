/* 
 * File:   client.cpp
 * Author: tlibal
 *
 * Created on January 4, 2016, 8:51 PM
 */

#include <cstdlib>
#include <iostream>
#include <string>

#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sstream>

#include "common/src/ProgramConfig.h"
#include "common/src/FileLog.h"
#include "common/src/uadet2.h"
#include "Network/src/Network.h"
#include "Client/src/Client.h"

#define BUFFERSIZE 2048

/*
 *
 */
int main(int argc, char** argv) {
    
    /*
     * 
     */
    Client mainClient;
    
    /*
     * File descriptor for the TCP open_connection
     * 
     */
    int sockfd;
    
    /*
     * Port number of the host machine to which the client is connecting
     * 
     */
    int portno;

    /*
     * The client program receives a command from the user which is read into this string
     * 
     */
    std::string command;

    /*
     * Host's information is stored in this struct.
     * 
     * struct hostent {
     *     char    *h_name;     // official name of the host
     *     char    **h_aliases; // alias list
     *     int     h_addrtype;  // host address type?
     *     int     h_length;    // length of the address
     *     char    **h_addr_list; // list of addresses from the name server
     *     #define h_addr h_addr_list[0] // address for backward compatibility
     * };
     */
    struct hostent *server = nullptr;
    
    /*
     * Delegate class to handle opening the TCP open_connection
     * 
     */
    Network network;
    
    /*
     * Initialize the file logger by setting the output log path to the current working directory
     * 
     */
    FileLog logger;  
    logger.setPath("./uadet2.client.log.txt");
    logger.log("Initializing Client");

    /*
     * Client Config
     *
     */
    std::string configFile = std::string("src/common/config/client.txt");
    ProgramConfigObject conf;
    ProgramConfig confCtrl(configFile);

    confCtrl.update(conf);
    mainClient.set_config_object(conf);

    logger.log("Got " + std::to_string(argc) + " arguments");
    
    /*
     * When argc == 4 the we assume this order of arguments:
     * 
     * uadet2cli hostname portno command
     */
    if (argc == 4) {
        command = std::string(argv[3]);
        portno = atoi(argv[2]);
        server = gethostbyname(argv[1]);
        
        logger.log(std::string(argv[1]));
        logger.log(std::string(argv[2]));
        logger.log(command);
    } else if (argc == 2) {
      /*
       * Otherwise the assumed order of arguments is this:
       * 
       * uadet2cli command
       */
        command = std::string(argv[1]);
        portno = conf.portno;
        server = gethostbyname(conf.hostname.c_str());
        
        logger.log(command);
    } else {
      /*
       * If argc != 4 && argc != 2 then throw an error and exit the program
       */
        logger.log("Invalid call without required arguments");
        
        std::cerr << "Usage: " << std::endl << "    " << argv[0] << " [<hostname> <port>] <command>" << std::endl;
        std::cerr << std::endl << "Example:" << std::endl;
        std::cerr << "    " << "uadet2cli 'eval mobile Mozilla/5.0 Linux Android'" << std::endl;
        std::cerr << "    " << "uadet2cli localhost 10128 'eval mobile Mozilla/5.0 Linux Android'" << std::endl;
        exit(EXIT_SUCCESS);
    }
    mainClient.process_arguments(argv, argc);
    
    sockfd = network.addr_connect(server);
    
    if (sockfd < 0) 
    {
        logger.log("ERROR opening socket");
        error("ERROR opening socket");
    }

    /*
     * Buffer for passing messages via TCP
     */
    char buffer[BUFFERSIZE];

    /*
     * Send over the command supplied by the user
     */
    int n = write(sockfd,command.data(),command.length());

    if (n < 0) {
        logger.log("ERROR writing to socket");
        error("ERROR writing to socket");
    }

    /*
     * Clear the buffer and read the response into the same buffer
     */
    bzero(buffer,BUFFERSIZE);
    n = read(sockfd,buffer,BUFFERSIZE);

    if (n < 0) {
        logger.log("ERROR reading from socket");
        error("ERROR reading from socket");
    }
    
    logger.log("Received response from the server");
    logger.log(std::string(buffer));
    std::cout << buffer << std::endl;

    logger.log("Exiting from main");
        
    return 0;
}

