#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <iostream>
#include <string>
#include "../../common/src/FileLog.h"

struct NetworkConfig {
    int const portno;
    int const backlogsize;
    std::string const hostname;
    
    NetworkConfig(int const portno = 10128, std::string const hostname = std::string("localhost"), int const backlogsize = 5) : portno(portno), hostname(hostname), backlogsize(backlogsize) {};
};

class Network {
public:
    Network(NetworkConfig const& netConfig);
    virtual ~Network();

    /**
     * sets the portno field
     */
    void set_port_no(int portno);

    /**
     * returns the portno field
     */
    int  get_port_no();
    
    /**
     * returns the hostname record
     */
    std::string get_hostname();

    /**
     * sets the FileLog logger
     */
    void set_file_log(FileLog *fileLog);

    /**
     * returns the FileLog logger used by this class
     */
    FileLog* get_file_log();

    /**
     * opens tcp connection, return the socket file descriptor
     */
    int  addr_connect(struct hostent *host);

    /**
     * binds to a port and listens for incoming connections
     */
    int addr_listen() const;
private:
    
    /*
     * 
     */
    NetworkConfig const& netConfig;
    
    /**
     * file backed logger
     */
    FileLog *fileLog = nullptr;

    /**
     * port number that will be used when creating tcp connections
     */
    int portno = 10128;

    /**
     * backlog size of connections queued to be processed
     */
    int backlogsize = 5;

    /**
     * logs the string message using the class' logger
     */
    void log(std::string const& msg) const;

    /**
     * creates a new socket and returns the socket fd
     */
    int create_socket_inet_stream() const;
};

#endif
