#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "../../common/src/ProgramConfig.h"
#include "../../common/src/FileLog.h"

#include <string>

/*
 * Client encapsulates data and methods for reading the stdin (command), 
 * including the options arguments and the config file, opening the 
 * connection to the server and sending and receiving the data from
 * the uadet2 server.
 * 
 */
class Client
{
public:
    Client();
    virtual ~Client();
    
    void set_config_object(ProgramConfigObject& config);
    ProgramConfigObject get_config_object();
    void process_arguments(char** argv, int argc);
private:
    std::string command = "";
    ProgramConfigObject config;
    FileLog logger;
    
    void log(const std::string msg);
};

#endif /* _CLIENT_H_ */