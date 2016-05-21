#include "Client.h"

#include <iostream>

Client::Client()
{
    
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
        //server = gethostbyname(argv[1]);
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
        //portno = conf.portno;
        //server = gethostbyname(conf.hostname.c_str());
        
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


