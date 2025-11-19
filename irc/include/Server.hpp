#ifndef SERVER_HPP
# define SERVER_HPP

#include "include.hpp"

#define CLIENT_MAX 10
/*
 handle listening socket
*/

class Server
{
	private:
		int socketFd;
		int port;
		std::string password;


		std::vector<struct pollfd> pollFd;

		struct sockaddr_in serverAddr;
	public:
		void initServer(std::string port, std::string password);
		void startServer();
		void newConnection();
		void handleClientData(int i);
		void disconnectClient(int i);
	
};



#endif