#ifndef SERVER_HPP
# define SERVER_HPP

#include "include.hpp"
/*
 handle listening socket
*/

class Server
{
	private:
		int socketFd;
		int port;
		std::string password;
	public:
		void initServer(std::string port, std::string password);
		void startServer();
	
};

#endif