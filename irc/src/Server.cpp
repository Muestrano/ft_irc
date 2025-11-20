#include "../include/Server.hpp"
#include "../include/include.hpp"


/**
 * @brief Create a client fd and initialize poll struct
 * @param accept ectract connection to create a new fd
 * @param fcntl open fd and configure it:
 * 
 * \li - F_SETFL: 
 * 
 * \li - O_NONBLOCK: 
*/
void Server::initServer(std::string port, std::string password)
{
	this->port = atoi(port.c_str());
	this->password = password; // need to cast it to have const password

	// init serv
	this->socketFd = socket(AF_INET, SOCK_STREAM, 0); //
	if (this->socketFd < 0)
		std::cerr << "Error: problem when creating socket" << std::endl;

	fcntl(this->socketFd, F_SETFL, O_NONBLOCK);

	this->serverAddr.sin_family = AF_INET;
	this->serverAddr.sin_port = htons(this->port);
	this->serverAddr.sin_addr.s_addr = INADDR_ANY;

	bind(this->socketFd, (struct sockaddr*)&this->serverAddr, sizeof(this->serverAddr));
	listen(this->socketFd, 10);

	struct pollfd listenerPollFd;
	listenerPollFd.fd = this->socketFd;
	listenerPollFd.events = POLLIN; // waiting rading (new connection)
	listenerPollFd.revents = 0;

	this->pollFd.push_back(listenerPollFd);


}
/**
 * @brief disconnect client and erase the client socket
*/
void Server::disconnectClient(int i)
{
	std::cout << "client disconnect: " << pollFd[i].fd << std::endl;
	close(this->pollFd[i].fd);
	this->pollFd.erase(pollFd.begin() + i);

	// clean associate client object and map client if there is

}

/**
 * @brief HexChat client pass three successive command to the serv to autentifiate and configure user
 * 
 * \li - it's: PASS, NICK, USER
*/
// void Server::initHexchat(int clientFd, const char* msg)
// {
// 	std::string totalMsg = msg;
// 	std::istringstream iss(totalMsg);
// 	std::string command;
// 	iss >> command;

// 	if (command == "PASS")
// 	{
// 		this->password = command;
// 	}

// 	iss >> command;
// 	if (command == "NICK")
// 	{
// 		this-> = 
// 	}
// }

/**
 * @brief handle receive message and parse the command
 * @param ssize_t Use to count bytes
*/
void Server::handleClientData(int i)
{
	int clientFd = pollFd[i].fd;
	char buffer[1024]; // Why 1024 ?? 

	while (true)
	{
		ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

		if (bytesRead > 0)
		{
			buffer[bytesRead] = '\0';
			// std::cout << "data client " << clientFd << ": '" << buffer << "'" << std::endl;
			// initHexchat(clientFd, buffer);
			// add data to client buff
			// parse the command (end by \r\n)
			// addDataClient(clientFd, buffer, bytesRead);
		}
		else if (bytesRead == 0)
		{
			disconnectClient(i);
			break;
		}
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			break; // no data to read
		else //error
		{
			disconnectClient(i);
			break;
		}
	}
}
/**
 * @param sockaddr_in socket adress ipv4 (sockaddr_in6 for ipv6)
*/
void Server::newConnection()
{
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

 	int clientFd = accept(socketFd, (struct sockaddr*)&addr, &addrlen);
	if (clientFd >= 0)
	{
		fcntl(clientFd, F_SETFL, O_NONBLOCK);
		struct pollfd newPollFd; // to push it in the vector pollFd and we aren't limits by number of user
		newPollFd.fd = clientFd;
		newPollFd.events = POLLIN | POLLOUT; // wire reading and writing
		newPollFd.revents = 0;

		this->pollFd.push_back(newPollFd);

		std::cout << "new client connected: " << clientFd << std::endl;
	}
}
/**
 * @param serverAddre socketaddr_in struct contain the server adress
 * @param AF_INET ipv4 protocol
 * @param SOCK_STREAM TCP socket
 * @param htons convert port to network byte ordre
 * @param INADDR_ANY accept all ip conncetion
 * 
*/
// poll evecnts: 
void Server::startServer()
{

	while (true)
	{
		int waitPoll = poll(pollFd.data(), pollFd.size(), -1);
		// check all socket with POLLIN if there up with revents ==0
		if (waitPoll > 0)
		{
			for (int i = pollFd.size() - 1; i >= 0 ; i--)
			{
				if (pollFd[i].revents == 0)
					continue;
				// if (pollFd[i].revents & POLLIN)
				if (pollFd[i].revents & POLLIN && pollFd[i].fd == socketFd)
				{
					newConnection();
					// if (pollFd[i].fd == socketFd) // new connection
					// else // data from an other client
				}
				else if (pollFd[i].revents & POLLIN)
					handleClientData(i);

				// chef if the socket is ready to send
				// if (pollFd[i].revents & POLLOUT)
				// 	// writeClient()
				// // check if there is error with POLLHUP and POLLER
				// if (pollFd[i].revents & (POLLHUP | POLLERR))
				// {
				// 	// disconnectClient(i)
				// 	i--; // for disconnection
				// }
			
			}
			// check all events new connection or data give for one client
		}
	}
}
/*
poll give info if the operand accept, recv, send can execute
fcntl with O_NONBLOCK flag to handle A/I operation
POLLIN
POLLOUT
POLLERR

	LOOP:
		-init pollFd struc with socket (client and listening)
		- call poll()
		- check all socket to see who's waiting for operation (send, recv...)
		create tab or vector for pollFd ?
*/
	
/*
socket : communication between two device in one network
	one socket for listening communication in one port

		main function:
		 -	socket() : socket creation
		 -	bind() : Ip and port association to one socket
		 -  listen() : put socket in listening mode to handle input connexion
		 -  send() / recv() : Send/receive data by the socket
		 -  close() : Close the socket

		All I/O operations non-blocking:

			Need to handle multiple socket in same time to haven't blocking operation.
			One server waiting to received msg is block
		 -	poll()/select()/kqueue()) : Wire a list of socket, he 'ping' when a socket is ready to be read, write, when new client connected ect...
		 		it's the main function loop
		 -	fcntl() with O_NONBLOCK : make a non-blocking socket. 
		 		if there is call of received and the socket is empty, fcntl handle the error


	

*/