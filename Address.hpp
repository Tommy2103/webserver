#ifndef __ADDRESS_H
#define __ADDRESS_H

#include <arpa/inet.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>

class Address
{
private:
  struct sockaddr_in address;
public:
  Address();
  Address(char* addr, int port);
  struct sockaddr_in getAddress();
  void setAddress(struct sockaddr_in address);
  void setIP(char* addr);
  char* getIP();
  void setPort(int port);
  int getPort();
  static char* getIpFromHostname(char*);
  char* toString();
};

Address::Address() : Address((char*) "127.0.0.1", 80) {}

Address::Address(char *addr, int port)
{
  address.sin_family = AF_INET;
  inet_aton(addr, &address.sin_addr);
  address.sin_port = htons(port);
}

struct sockaddr_in Address::getAddress()
{
    return this->address;
}

void Address::setAddress(struct sockaddr_in addr)
{
    this->address = addr;
}

void Address::setIP(char *addr)
{
  inet_aton(addr, &address.sin_addr);
}

char *Address::getIP()
{
  return strdup(inet_ntoa(address.sin_addr));
}

void Address::setPort(int port)
{
  address.sin_port = htons(port);
}

int Address::getPort()
{
  return ntohs(address.sin_port);
}

char* Address::getIpFromHostname(char* hostname) {
  struct hostent* host = gethostbyname(hostname);
  if (host == NULL) {
    switch (h_errno) {
      case HOST_NOT_FOUND: 
        printf("host not found\n");
        break;
      case NO_DATA:
        printf("no ip\n");
        break;
    }
    return NULL;
  } 
  in_addr* address = (in_addr*) host->h_addr;
  
  return strdup(inet_ntoa(*address));

}

char *Address::toString()
{
  char *addr = getIP();
  char ret[21];
  sprintf(ret, "%s:%d", addr, getPort());
  free(addr);
  return strdup(ret);
}


#endif
