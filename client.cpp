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

/*
 * Prints the error message and exits the program with the implementation
 * dependent EXIT_FAILURE exit code.
 */
void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

/*
 * 
 */
int main(int argc, char** argv) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    
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
    struct hostent *server;
    
    if (argc < 4)
    {
        std::cerr << "Usage: " << std::endl << "    " << argv[0] << " <hostname> <port> <command>" << std::endl;
        std::cerr << std::endl << "Example:" << std::endl;
        std::cerr << "    " << "uadet2cli localhost 10128 'eval mobile Mozilla/5.0 Linux Android'" << std::endl;
        exit(EXIT_SUCCESS);
    }
    
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        error("ERROR opening socket");
    }
    
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        error("ERROR, no such host");        
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    
    if (::connect(sockfd,(const sockaddr*) &serv_addr,sizeof(serv_addr)) < 0)
    {
        error("ERROR connecting");
    }
    
    char buffer[256];
    std::string command = argv[3];
    
    n = write(sockfd,command.data(),command.length());
    
    if (n < 0)
    {
        error("ERROR writing to socket");
    }
    
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    
    if (n < 0)
    {
        error("ERROR reading from socket");
    }
    
    std::cout << "server's response was " << buffer << std::endl;
    
    return 0;
}

