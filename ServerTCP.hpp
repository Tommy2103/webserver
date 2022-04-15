#ifndef __SERVERTCP_HPP
#define __SERVERTCP_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <list>
#include "utilities.hpp"
#include "Address.hpp"
#include "SocketTCP.hpp"
#include "Connection.hpp"

class ServerTCP : public SocketTCP
{
public:
	ServerTCP(std::string address, int port);
	Connection* listen();
};


ServerTCP::ServerTCP(std::string address, int port) : SocketTCP() {

	Address server_addr(address, port);
	struct sockaddr_in bind_addr = server_addr.getAddress();
	int rc = bind(this->sock_id, 
		(struct sockaddr*) &bind_addr, 
		(socklen_t) sizeof(struct sockaddr));
	if (rc) errore((char*) "bind()", -2);
}

Connection* ServerTCP::listen() {
	
	int rc = ::listen(sock_id, SOMAXCONN);
	if (rc) errore((char*) "listen()", -3);

    
	struct sockaddr_in addr;
	int len = sizeof(struct sockaddr);
	int conn_id = accept(sock_id, (struct sockaddr*) &addr, (socklen_t*) &len);
	if (conn_id == -1) return NULL;

	return new Connection(conn_id);
}

#endif
