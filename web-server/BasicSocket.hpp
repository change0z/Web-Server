#ifndef BasicSocket_hpp
#define BasicSocket_hpp

#include <stdio.h>
#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <iostream>

namespace WBS {
	class BasicSocket
	{
	private:
		struct sockaddr_in address;
		int sock; //this was declared so that the connection variable does not disappear and it is private because we dont want to be actively changing it.
		int connection;
	public:
		// Constructor
		BasicSocket(int domain, int service, int protocol, int port, u_long h_interface);
		// Virtual Function to conncet to a newtwork
		virtual int connect_to_network(int sock, struct sockaddr_in address) = 0;
		// Function to test sockets and connections
		void test_connection(int);
		// Getter Functions
		struct sockaddr_in get_address();
		int get_sock();
		int get_connection();
	};
}

#endif /* BasicSocket_hpp */
