#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "../../common/src/FileLog.h"

class Network {
 public:
  Network();
  virtual ~Network();

  void set_port_no(int portno);
  int  get_port_no();
  
  void set_file_log(FileLog *fileLog);
  FileLog* get_file_log();
  
  /**
   * opens tcp connection, return the socket file descriptor
   */
  int  open_connection(struct hostent *host);
 private:
  FileLog *fileLog = nullptr;
  int sockfd;
  int portno;
  struct sockaddr_in *serv_addr;
  struct hostent *server;
};

#endif
