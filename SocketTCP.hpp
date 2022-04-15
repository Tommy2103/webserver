#ifndef __SOCKETTCP_HPP
#define __SOCKETTCP_HPP

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include "Address.hpp"
#include "utilities.hpp"

class SocketTCP
{
protected:
	int sock_id;
public:
	SocketTCP();
	~SocketTCP();
};

SocketTCP::SocketTCP() {
	this->sock_id = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_id == -1) errore((char*)"socket()", -1);
}

SocketTCP::~SocketTCP() {
	close(this->sock_id);
}

#endif
