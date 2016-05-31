#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "../../common/src/ProgramConfig.h"
#include "../../common/src/FileLog.h"
#include "../../Network/src/Network.h"

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
    void start();
private:
    /*
     * The client program receives a command from the user which is read into this string
     * 
     */
    std::string command = "";
    
    std::string const configFilePath = "config/client.txt";
    
    ProgramConfigObject config;
    
    FileLog logger;
        
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
    
    void log(std::string const& msg);
};

#endif /* _CLIENT_H_ */