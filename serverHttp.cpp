#include "Connection.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include "ServerTCP.hpp"
#include "utilities.hpp"
#include "Cgi.hpp"

#define ADDR "0.0.0.0"
#define ROOT_FOLDER "./www"

void main_thread(int);
void connection_thread(Connection*);
char* handleRequest(char*, char*);


int main(int argc, char const *argv[])
{
	if (argc != 2) {
		std::cout << "USAGE: " << argv[0] << " [Port]\n";
		errore("argv", -1);
	}

	int port = atoi(argv[1]);
	std::thread main(main_thread, port);
	main.detach();
	std::cout << "premi un tasto per interrompere il server...\n";
	getchar();
	return 0;
}

void main_thread(int port) {
	ServerTCP server((char*)ADDR, port);

	while (1) {
		Connection* conn = server.listen();
		
		std::thread connection(connection_thread, conn);
		connection.detach();
		delete conn;
	}


}

void connection_thread(Connection* conn) {

	std::string req = conn->receive();

	std::cout << "response: " << req.size() << " byte\n\n";
	std::cout << req << "\n";

	char* toParse = (char*) req.c_str();
	char* requestType = strtok(toParse, " ");
	char* requestRoute = strtok(NULL, " ");
	
	char* formattedRoute = (char*) malloc(strlen(requestRoute)+strlen(ROOT_FOLDER)+1);
	sprintf(formattedRoute, "%s%s", ROOT_FOLDER, requestRoute);

	char* res = handleRequest(requestType, formattedRoute);
	
	//char* cgiRes = Cgi::parse(res);
	conn->send(res);
	//free(cgiRes);
	free(res);
	free(formattedRoute);
	delete conn;
}

char* handleRequest(char* requestType, char* requestRoute) {
	if (!strcmp(requestType, "GET")) {
		FILE* file = fopen(requestRoute, "r");
		if (file == NULL)
			return strdup("HTTP 404 NOT FOUND\r\n\r\n"); 
		
		int size = getFileSize(file);
		char* content = (char*)malloc(size);
		fread(content, sizeof(char), size, file);
		fclose(file);
		return content;
	}
	return strdup("Request type not yet supported");
}





#pragma GCC diagnostic pop