#ifndef BindingSocket_hpp
#define BindingSocket_hpp






#include <stdio.h>
#include "BasicSocket.hpp"

namespace WBS {
	class BindingSocket : public BasicSocket {
	public:
		BindingSocket(int domain, int service, int protocol, int port, u_long h_interface) : BasicSocket(domain, service, protocol, port, h_interface);
		int connect_to_network(int sock, struct sockaddr_in adderss);
	};
}



#endif // !BindingSocet_hpp