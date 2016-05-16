#ifndef _NETWORK_H_
#define _NETWORK_H_

class Network {
 public:
  Network();
  virtual ~Network();

  void set_port_no(int portno);
  int  get_port_no();
  
  /**
   * opens tcp connection, return the socket file descriptor
   */
  int  open_connection(struct hostent *host);
 private:
  int sockfd;
  int portno;
  struct sockaddr_in *serv_addr;
  struct hostent *server;
};

#endif
