#ifndef __ADDRESS_H
#define __ADDRESS_H

#include <arpa/inet.h>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>

class Address
{
private:
  struct sockaddr_in address;
public:
  Address();
  Address(std::string addr, int port);
  struct sockaddr_in getAddress();
  void setAddress(struct sockaddr_in address);
  void setIP(std::string addr);
  std::string getIP();
  void setPort(int port);
  int getPort();
  static std::string getIpFromHostname(std::string);
  std::string toString();
};

Address::Address() : Address( "127.0.0.1", 80) {}

Address::Address(std::string addr, int port)
{
	address.sin_family = AF_INET;
	inet_aton(addr.c_str(), &address.sin_addr);
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

void Address::setIP(std::string addr)
{
	inet_aton(addr.c_str(), &address.sin_addr);
}

std::string Address::getIP()
{
	return inet_ntoa(address.sin_addr);
}

void Address::setPort(int port)
{
	address.sin_port = htons(port);
}

int Address::getPort()
{
	return ntohs(address.sin_port);
}

std::string Address::getIpFromHostname(std::string hostname) {
	struct hostent* host = gethostbyname(hostname.c_str());
	if (host == NULL) {
		switch (h_errno) {
			case HOST_NOT_FOUND: 
			printf("host not found\n");
			break;
			case NO_DATA:
			printf("no ip\n");
			break;
		}
		return "";
	} 
	in_addr* address = (in_addr*) host->h_addr;

	return inet_ntoa(*address);
}

std::string Address::toString()
{
  return getIP() + ":" + std::to_string(getPort());
}


#endif
