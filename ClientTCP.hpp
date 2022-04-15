#ifndef __CLIENTTCP_HPP
#define __CLIENTTCP_HPP

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string>
#include "Address.hpp"
#include "SocketTCP.hpp"
#include "Connection.hpp"

class ClientTCP : public SocketTCP
{
private:
    Connection* conn = NULL;
public:
	ClientTCP();
    ~ClientTCP();
	bool connect(Address server);
	bool send(std::string msg);
	bool send(void* msg, int len);
	std::string receive();
	void* receive(int* len);
};


ClientTCP::ClientTCP() : SocketTCP() { }

ClientTCP::~ClientTCP() {
    delete this->conn;
}

bool ClientTCP::connect(Address server) {
	struct sockaddr_in addr = server.getAddress();
	int rs = ::connect(this->sock_id, (struct sockaddr*) &addr, (socklen_t) sizeof(struct sockaddr));
	
	if (rs) return true;
    this->conn = new Connection(this->sock_id);
	return false;
}

bool ClientTCP::send(std::string msg) {
	return this->conn->send(msg);
}

bool ClientTCP::send(void* msg, int len) {
	return this->conn->send(msg, len);
}

std::string ClientTCP::receive() {
	return this->conn->receive();
}

void* ClientTCP::receive(int* len) {
	return this->conn->receive(len);
}

#endif
