#ifndef __CLIENTTCP_HPP
#define __CLIENTTCP_HPP

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
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
	bool connetti(Address server);
	bool invia(char* msg);
	bool invia(void* msg, int len);
	char* ricevi();
	void* ricevi(int* len);
};


ClientTCP::ClientTCP() : SocketTCP() { }

ClientTCP::~ClientTCP() {
    delete this->conn;
}

bool ClientTCP::connetti(Address server) {
	struct sockaddr_in addr = server.getAddress();
	int rs = connect(this->sock_id, (struct sockaddr*) &addr, (socklen_t) sizeof(struct sockaddr));
	
	if (rs) return true;
    this->conn = new Connection(this->sock_id);
	return false;
}

bool ClientTCP::invia(char* msg) {
	return invia(msg, strlen(msg));
}

bool ClientTCP::invia(void* msg, int len) {
	return this->conn->invia(msg, len);
}

char* ClientTCP::ricevi() {
	return this->conn->ricevi();
}

void* ClientTCP::ricevi(int* len) {
	return this->conn->ricevi(len);
}

#endif
