#include "Network.h"

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
