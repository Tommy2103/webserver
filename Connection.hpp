#ifndef __CONNECTION_HPP
#define __CONNECTION_HPP

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
	bool invia(char* msg);
	bool invia(void* msg, int len);
	char* ricevi();
	void* ricevi(int* len);
	int getId();
	void setId(int id);
};


Connection::Connection(int id) {
	this->id = id;
}

Connection::~Connection() {
	int rc = shutdown(getId(), SHUT_RDWR);
}

bool Connection::invia(char* msg) {
	return invia(msg, strlen(msg)+1);
}

bool Connection::invia(void* msg, int len) {

	int rs = send(this->id, msg, len, 0);
	if (rs == -1) return true;
	return false;
}

char* Connection::ricevi() {
	int len = MAX_LEN;
	char* res = (char*) ricevi(&len);
	return res;
}

void* Connection::ricevi(int* len) {
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