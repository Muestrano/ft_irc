#ifndef SERVER_HPP
# define SERVER_HPP

#include "include.hpp"

/*
 handle listening socket
*/

class Client;

class Server
{
	private:
		int socketFd;
		int port;
		std::string password;


		std::vector<struct pollfd> pollFd;

		struct sockaddr_in serverAddr;
	public:
		Server(int port, std::string password);
		Server();


		void initServer();
		void initHexchat(int clientFd, const char* buffer);
		void startServer();
		void newConnection();
		void handleClientData(int i);
		void disconnectClient(int i);
		// std::map<int, Client*> clients;
	
};



#endif