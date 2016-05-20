#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <iostream>
#include "../../common/src/FileLog.h"

class Network {
public:
    Network();
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
    int addr_listen();
private:
    /**
     * file backed logger
     */
    FileLog *fileLog = nullptr;

    /**
     * field that holds the socket fd
     */
    int sockfd = -1;

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
    void log(const std::string& msg);

    /**
     * creates a new socket and returns the socket fd
     */
    int create_socket_inet_stream();
};

#endif
