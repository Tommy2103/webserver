#ifndef __CONNECTION_HPP
#define __CONNECTION_HPP

#include <cstddef>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include "Address.hpp"

#define MAX_LEN 10000

class Connection
{
private:
	int id;
public:
	Connection(int id);
	~Connection();
	bool send(std::string msg);
	bool send(void* msg, int len);
	std::string receive();
	void* receive(int* len);
	int getId();
	void setId(int id);
};


Connection::Connection(int id) {
	this->id = id;
}

Connection::~Connection() {
	int rc = shutdown(getId(), SHUT_RDWR);
}

bool Connection::send(std::string msg) {
	char const* c_msg = msg.c_str();
	return this->send((void*) c_msg, strlen(c_msg));
}

bool Connection::send(void* msg, int len) {
	int rs = ::send(this->id, msg, len, 0);
	if (rs == -1) return true;
	return false;
}

std::string Connection::receive() {
	int len = MAX_LEN;
	char* s = (char*) receive(&len);
	std::string res(s);
	free(s);
	return res;
}

void* Connection::receive(int* len) {
	void* msg = malloc(*len);

	int rs = recv(this->id, msg, *len, 0);
	
	*len = rs;
	if (rs == -1 || rs == 0) {
		free(msg);
		return NULL;
	}
	return realloc(msg, rs);
}

int Connection::getId() { return id; }

void Connection::setId(int id) { this->id = id; }


#endif