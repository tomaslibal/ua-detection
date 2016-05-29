#include "Network.h"
#include "../../common/src/uadet2.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <sstream>
#include <chrono>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>


#define h_addr h_addr_list[0] // address for backward compatibility

Network::Network()
{
}

Network::~Network()
{
}

int Network::create_socket_inet_stream() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR Opening Socket");
    }
    return sockfd;
}

void Network::set_port_no(int portno)
{
    this->portno = portno;
}

int Network::get_port_no()
{
    return portno;
}

int Network::addr_connect(struct hostent *host)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        std::string errmsg = "ERROR opening socket";
        log(errmsg);
        error(errmsg.c_str());
    }

    if (host == NULL)
    {
        std::string errmsg = "ERROR, no such host";
        log(errmsg);
        error(errmsg.c_str());
    }

    sockaddr_in serv_addr;

    /*
     * Clear the data
     */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    /*
     * Set Internet Address Family 
     */
    serv_addr.sin_family = AF_INET;
    
    /*
     * Set Server IP address
     */
    bcopy((char *)host->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, host->h_length);
    
    /*
     * Set server port number
     */
    serv_addr.sin_port = htons(portno);

    /*
     * Establish the connection
     */
    if (::connect(sockfd, (const sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        std::string errmsg = "ERROR connecting";
        log(errmsg);
        error(errmsg.c_str());
    }

    return sockfd;
}

int Network::addr_listen()
{
    int sockfd;

    sockfd = create_socket_inet_stream();

    if (sockfd < 0) {
        error("Creating a socket failed");
        log("Creating a socket failed");
    }

    sockaddr_in serv_addr;

    bzero((char *) &serv_addr, sizeof(serv_addr));

    log("Using port number " + std::to_string(portno));

    serv_addr.sin_family = AF_INET;
    // host address: INADDR_ANY
    // The socket will receive packets for all network interfaces of the host machine on the given port number
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    int retries = 12;
    int result = -1;
    int wait = 3;

    while (retries > 0 && (result = ::bind(sockfd, (sockaddr *) &serv_addr, sizeof(serv_addr))) < 0) {
        retries--;
        std::cerr << "error binding, retrying in " << wait << "s..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(wait));
    }

    if (result < 0) {
        error("binding failed");
    }

    log("Binding to the socket successful");
    std::cout << "binding to the socket successful on port " << portno << std::endl;

    /*
      * Accept incoming connections
      */
    int ls = listen(sockfd, backlogsize);

    if (ls < 0) {
        error("cannot accept connection");
        log("cannot accept connections [::listen]");
    }

    return sockfd;
}


FileLog* Network::get_file_log()
{
    return fileLog;
}

void Network::set_file_log(FileLog* fileLog)
{
    this->fileLog = fileLog;
}

void Network::log(const std::string& msg)
{
    if (fileLog) {
        fileLog->log(msg);
    }
}


