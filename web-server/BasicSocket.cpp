#include "BasicSocket.hpp"

// Default Constructor

WBS::BasicSocket::BasicSocket(int domain, int service, int protocol, int port, u_long h_interface) {
	// Define address structure
	address.sin_family = domain;
	address.sin_port = htons(port); //we are giving an integer to the port whose bytes are according to the hosts system, we need to set them for the actuall network
	address.sin_addr.s_addr = htonl(h_interface);
	// Establish Connection
	sock = socket(domain, service, protocol);
	test_connection(sock);
	// Establish Network Connection
	connection = connect_to_network(sock, address);
	test_connection(connection);
}

// Test Connection Virtual Function

void WBS::BasicSocket::test_connection(int item_to_test) {
	// Confirms that the socket or connection has been properly established
	if (item_to_test < 0) {
		perror("Failed to connect...");
		exit(EXIT_FAILURE);
			
	}
}

// Getter Function 

struct sockaddr_in WBS::BasicSocket::get_address(){
	return address;
}

int WBS::BasicSocket::get_sock()
{
	return sock;
}

int WBS::BasicSocket::get_connection() {
	return connection;
}