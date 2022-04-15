#include <stdio.h>
#include <regex>
#include "ClientTCP.hpp"
#include "utilities.hpp"

#define REGEX_IPV4 "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"
#define MAX_REQ 500

int main(int argc, char *argv[])
{
 	
	if (argc != 4) {
		printf((char*)"USAGE: %s [URL/IPv4] [Route] [Server port]\n", argv[0]);
		errore((char*)"argv", -1);
	}
	

	ClientTCP client;
	char* host;
	if (std::regex_match(argv[1], std::regex(REGEX_IPV4))) {
		host = argv[1];
	} else {
		host = Address::getIpFromHostname(argv[1]);
		if (host == NULL) errore("getIpFromHostname()", -2);
	}
	Address server(host, atoi(argv[3]));
	
	if (client.connetti(server)) errore("connect()", -3);
	char request[MAX_REQ];
	sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\nAccept: text/html\r\n\r\n", argv[2], argv[1]);
	client.invia(request);
	
	char* res; 

	while ((res = client.ricevi()) != NULL) {
		printf("---------\n%d byte\n---------\n", strlen(res));
		printf("%s\n", res);
		free(res);
	}

	

	return 0;
}
