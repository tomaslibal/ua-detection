#include "Network.h"
#include "../../common/src/uadet2.h"

#include <cstdlib>
#include <iostream>
#include <string>

#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sstream>

#define h_addr h_addr_list[0] // address for backward compatibility

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
  //serv_addr = new sockaddr_in();
  //serv_addr->sin_addr = new in_addr();

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)host->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, host->h_length);
  serv_addr.sin_port = htons(portno);

  if (::connect(sockfd,(const sockaddr*) &serv_addr,sizeof(serv_addr)) < 0) {        
    std::string errmsg = "ERROR connecting";
    log(errmsg);
    error(errmsg.c_str());
  }
    
  //delete serv_addr;
    
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


