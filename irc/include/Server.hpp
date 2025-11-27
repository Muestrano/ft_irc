#ifndef SERVER_HPP
# define SERVER_HPP

#include "include.hpp"
#include "Command.hpp"

class Client;


class Server
{
	private:

		int 		socketFd;
		int 		port;
		std::string password;
		Command		cmd;
		
		std::vector<struct pollfd> pollFd;
		std::map<int, Client*> clients;  // FD -> Client
    	// std::map<std::string, Channel*> channels; TODO

		struct sockaddr_in serverAddr;
	
	public:

		Server(int port, std::string password);
		~Server();
		
		// Getter
		std::string getPassword() const;

		// Public method

		void initServer();
		void extractCompleteCommand(Client* client);
		void prepareCommand(Client* client, std::string line);
		void startServer();
		void newConnection();
		void handleClientData(int i);
		void disconnectClient(int i);

};



#endif