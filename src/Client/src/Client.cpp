#include "Client.h"

#include <iostream>
#include <sstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>


#define BUFFERSIZE 2048

Client::Client()
{
    logger.setPath("./uadet2.client.log.txt");
    log("Initializing Client");
    
    ProgramConfig confCtrl(configFilePath);
    confCtrl.update(config);
}

Client::~Client()
{

}

void Client::log(const std::string msg)
{
    logger.log(msg);
}


void Client::set_config_object(ProgramConfigObject& config)
{
    this->config = config;
}

ProgramConfigObject Client::get_config_object()
{
    return config;
}

void Client::process_arguments(char** argv, int argc)
{
    /*
     * When argc == 4 the we assume this order of arguments:
     * 
     * uadet2cli hostname portno command
     */
    if (argc == 4) {
        command = std::string(argv[3]);
        config.portno = atoi(argv[2]);
        //portno = atoi(argv[2]);
        server = gethostbyname(argv[1]);
        config.hostname = argv[1];
        
        log(std::string(argv[1]));
        log(std::string(argv[2]));
        log(command);
    } else if (argc == 2) {
      /*
       * Otherwise the assumed order of arguments is this:
       * 
       * uadet2cli command
       */
        command = std::string(argv[1]);
        //portno = config.portno;
        server = gethostbyname(config.hostname.c_str());
        
        log(command);
    } else {
      /*
       * If argc != 4 && argc != 2 then throw an error and exit the program
       */
        log("Invalid call without required arguments");
        
        std::cerr << "Usage: " << std::endl << "    " << argv[0] << " [<hostname> <port>] <command>" << std::endl;
        std::cerr << std::endl << "Example:" << std::endl;
        std::cerr << "    " << "uadet2cli 'eval mobile Mozilla/5.0 Linux Android'" << std::endl;
        std::cerr << "    " << "uadet2cli localhost 10128 'eval mobile Mozilla/5.0 Linux Android'" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Client::start()
{    
    network.set_port_no(config.portno);
    
    int sockfd = network.addr_connect(server);
    
    if (sockfd < 0) 
    {
        log("ERROR opening socket");
        //error("ERROR opening socket");
        return;
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
        log("ERROR writing to socket");
        //error("ERROR writing to socket");
        return;
    }

    /*
     * Clear the buffer and read the response into the same buffer
     */
    bzero(buffer,BUFFERSIZE);
    n = read(sockfd,buffer,BUFFERSIZE);

    if (n < 0) {
        log("ERROR reading from socket");
       // error("ERROR reading from socket");
        return;
    }
    
    log("Received response from the server");
    log(std::string(buffer));
    std::cout << buffer << std::endl;

    log("Exiting from main");
}


