#ifndef _NETWORK_H_
#define _NETWORK_H_

class Network {
 public:
  Network();
  virtual ~Network();

  void set_port_no(int portno);
  int  get_port_no();
 private:
  int sockfd;
  int portno;
};

#endif
