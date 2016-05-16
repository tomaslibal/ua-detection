#include "Network.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <cstring>

Network::Network()
{
}

Network::~Network()
{
}

void Network::set_port_no(int portno)
{
  this->portno = portno; 
}

int Network::get_port_no()
{
  return portno;
}

int Network::open_connection(struct hostent *host)
{
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  
/*  if (sockfd < 0) 
  {
    logger.log("ERROR opening socket");
    error("ERROR opening socket");
  }

  if (server == NULL)
  {
    logger.log("ERROR, no such host");
    error("ERROR, no such host");
  }
  
  */

  bzero((char *) serv_addr, sizeof(serv_addr));
  serv_addr->sin_family = AF_INET;
  bcopy((char *)host->h_addr, (char *)serv_addr->sin_addr.s_addr, host->h_length);
  serv_addr->sin_port = htons(portno);

  if (::connect(sockfd,(const sockaddr*) serv_addr,sizeof(serv_addr)) < 0) {        
 //   logger.log("ERROR connecting");
 //   error("ERROR connecting");
  }
    
  return sockfd;
}
