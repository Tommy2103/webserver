#include <iostream>
#include <regex>
#include <string>
#include "ClientTCP.hpp"
#include "utilities.hpp"

#define REGEX_IPV4 "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"
#define MAX_REQ 500

int main(int argc, char *argv[])
{
 	
	if (argc != 4) {
		std::cout << "USAGE: " << argv[0] << " [URL/IPv4] [Route] [Server port]\n";
		errore((char*)"argv", -1);
	}
	

	ClientTCP client;
	std::string host;
	if (std::regex_match(argv[1], std::regex(REGEX_IPV4))) {
		host = argv[1];
	} else {
		host = Address::getIpFromHostname(argv[1]);
		if (host.empty()) errore("getIpFromHostname()", -2);
	}
	Address server(host, atoi(argv[3]));
	if (client.connect(server)) errore("connect()", -3);

	char request[MAX_REQ];
	sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\nAccept: text/html\r\n\r\n", argv[2], argv[1]);
	client.send(request);
	
	std::string res;

	while (!(res = client.receive()).empty()) {
		std::cout << "---------\n" << res.size() << " byte\n---------\n";
		std::cout << res << "\n";
	}

	

	return 0;
}
