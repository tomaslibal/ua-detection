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
     * returns the portno field
     */
    int  get_port_no() const;
    
    /*
     * Returns the maximum number of waiting incoming TCP connections
     */
    int get_backlogsize() const;
    
    /**
     * returns the hostname record
     */
    std::string get_hostname() const;

    /**
     * sets the FileLog logger
     */
    void set_file_log(FileLog *fileLog);

    /**
     * returns the FileLog logger used by this class
     */
    FileLog* get_file_log() const;

    /**
     * opens tcp connection, return the socket file descriptor
     */
    int  addr_connect(struct hostent *host) const;

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
     * logs the string message using the class' logger
     */
    void log(std::string const& msg) const;

    /**
     * creates a new socket and returns the socket fd
     */
    int create_socket_inet_stream() const;
};

#endif
