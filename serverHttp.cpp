#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "ServerTCP.hpp"
#include "utilities.hpp"
#include "Cgi.hpp"

#define ADDR "0.0.0.0"

void* main_thread(void*);
void* connection_thread(void*);
char* handleRequest(char*, char*);


int main(int argc, char const *argv[])
{
	if (argc != 2) {
		printf("USAGE: %s [Port]\n", argv[0]);
		errore("argv", -1);
	}

	pthread_t tid;
	int port = atoi(argv[1]);
	pthread_create(&tid, NULL, main_thread, (void*)&port);

	printf("premi un tasto per interrompere il server...\n");
	getchar();

	return 0;
}

void* main_thread(void* param) {
	int port = *(int*)param;
	ServerTCP server((char*)ADDR, port);
	
	while (1) {
		Connection* conn = server.ascolta();
		
		pthread_t tid_connection;
		pthread_create(&tid_connection, NULL, connection_thread, (void*) conn);
		pthread_detach(tid_connection);	
	}


}

void* connection_thread(void* param) {
	Connection* conn = (Connection*) param;

	char* req = conn->ricevi();

	printf("response: %d byte\n\n", strlen(req));
	printf("%s\n", req);

	char* toParse = strdup(req);
	char* requestType = strtok(toParse, " ");
	char* requestRoute = strtok(NULL, " ");
	
	char* formattedRoute = (char*) malloc(strlen(requestRoute)+1);
	sprintf(formattedRoute, ".%s", requestRoute);

	char* res = handleRequest(requestType, formattedRoute);
	
	char* cgiRes = Cgi::parse(res);
	conn->invia(cgiRes);

	free(cgiRes);
	free(res);
	free(formattedRoute);
	free(toParse);
	free(req);
	delete conn;
	pthread_exit(NULL);
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