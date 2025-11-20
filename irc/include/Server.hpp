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


		std::vector<struct pollfd> pollFd;

		struct sockaddr_in serverAddr;
	public:
		void initServer(std::string port, std::string password);
		void initHexchat(int clientFd, const char* buffer);
		void startServer();
		void newConnection();
		void handleClientData(int i);
		void disconnectClient(int i);
	
};



#endif