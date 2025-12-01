#include "../include/Server.hpp"
#include "../include/include.hpp"

/**
 * @param AF_INET ipv4 protocol
 * @param SOCK_STREAM TCP socket
 */
Server::Server(int port, std::string pass)
{
	this->socketFd = socket(AF_INET, SOCK_STREAM, 0);
	this->port = port;
	this->password = pass;
	cmd.setServer(this);
}
Server::~Server()
{
	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
		delete it->second; // delete each Client
	clients.clear();
	
	if (socketFd >= 0)
		close(socketFd);
}

// Getter

std::string Server::getPassword() const
{
	return this->password;
}

// Public method

/**
 * @brief Create a server fd and initialize poll/sockaddr struct
 * @param fcntl open fd and configure it:
 * 
 * \li - F_SETFL: Set File Status Flags, overwrites actual flags
 * 
 * \li - O_NONBLOCK: flag non blocking to accept(), connect(), recv(), et send(). 
 * 			if no data socket function return directly −1 and global variable errno is define to EAGAIN or EWOULDBLOCK.
 * @param sockadrr => serverAddr component
 * 
 * \li - AF_INET type of adress, this specify the ipv4 adress (AF_INET6 for ipv6)
 * 
 * \li - htons(this->port): Host To Network Short, convert port to short int (16 bit) for the server to read it => cf big enddian / little endian
 * 
 * \li - INADDR_ANY: The serv accept all local network interface (wifi, internet, loopback)
 * 
 * @param bind() : Ip and port association to one socket
 * 
 * @param listen() : put socket in listening mode to handle input connexion
 * 
 * \li - the backlog is 10 because we don't handle lot of connection in this project.
 * 			backlog is a queue if to handle connection peak
 * @param pollstruct handle events and re-events: wait to read, write, error with appropriate flags
*/
void Server::initServer()
{
	// init serv
	if (this->socketFd < 0)
		std::cerr << "Error: problem when creating socket" << std::endl;

	fcntl(this->socketFd, F_SETFL, O_NONBLOCK);
	// ----------TEMP potentialy rm this part if we handle signal--------------
	int reuse = 1;
	if (setsockopt(this->socketFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		std::cerr << "Error setsockopt(SO_REUSEADDR): " << std::endl;
		close(this->socketFd);
		exit(1);
	}
	// ----------TEMP potentialy rm this part if we handle signal--------------

	this->serverAddr.sin_family = AF_INET;
	this->serverAddr.sin_port = htons(this->port);
	this->serverAddr.sin_addr.s_addr = INADDR_ANY;

	bind(this->socketFd, (struct sockaddr*)&this->serverAddr, sizeof(this->serverAddr));
	listen(this->socketFd, 10);

	struct pollfd listenerPollFd;
	listenerPollFd.fd = this->socketFd;
	listenerPollFd.events = POLLIN; // waiting reading (new connection)
	listenerPollFd.revents = 0;

	this->pollFd.push_back(listenerPollFd);
}

/**
 * @brief disconnect client and erase the client socket
*/
void Server::disconnectClient(int i)
{
	int clientFd = pollFd[i].fd;
	std::cout << "client disconnect: " << clientFd << std::endl;
	if (clients.find(clientFd) != clients.end()) 
	{
		delete clients[clientFd];    // free client
		clients.erase(clientFd);     // clean map
	}
	
	close(clientFd);
	pollFd.erase(pollFd.begin() + i);

}

/**
 * @brief handle receive message and parse the command
 * @param ssize_t Use to count bytes
 * @param buffer 1024 octets => conventional size to read TCP (three control protocol) data
*/
void Server::handleClientData(int i)
{
	int clientFd = pollFd[i].fd;
	Client* client(this->clients[clientFd]);
	char buffer[1024];

	ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
	if (bytesRead > 0)
	{
		buffer[bytesRead] = '\0';
		client->setBuffer(buffer);
		cmd.extractCompleteCommand(client);
		// client->clearbuff() // TODO need to see if it's necessary
	}
	else if (bytesRead == 0)
	{
		disconnectClient(i);
	}
	else //error
	{ 
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return;
		else 
		{
			disconnectClient(i);
		}
	}
}

/**
 * @param sockaddr_in socket struct adress ipv4 (sockaddr_in6 for ipv6)
 * @param accept extract connection to create a new fd with the three step handshake (SYN: Synchronize, SYN-ACK: Synchronize-Acknowledge, ACK: Acknowledge)
*/
void Server::newConnection()
{
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

 	int clientFd = accept(socketFd, (struct sockaddr*)&addr, &addrlen);
	if (clientFd >= 0)
	{
		Client* newClient = new Client(clientFd);
		this->clients[clientFd] = newClient;
		fcntl(clientFd, F_SETFL, O_NONBLOCK);
		struct pollfd newPollFd; // to push it in the vector pollFd and we aren't limits by number of user
		newPollFd.fd = clientFd;
		newPollFd.events = POLLIN | POLLOUT; // wire reading and writing
		newPollFd.revents = 0;

		this->pollFd.push_back(newPollFd);
		std::cout << "new client connected: " << clientFd << std::endl;
	}
	else
	{
		  if (errno != EAGAIN && errno != EWOULDBLOCK)
		  	std::cerr << "Erreur accept(): " << std::endl;
	}
}

/**
 * TODO Doxygen comment
*/
void Server::startServer()
{

	while (true)
	{
		int waitPoll = poll(pollFd.data(), pollFd.size(), 5000);
		// check all socket with POLLIN if there up with revents ==0
		if (waitPoll > 0)
		{
			for (int i = pollFd.size() - 1; i >= 0 ; i--)
			{
				if (pollFd[i].revents == 0)
					continue;
				if (/*pollFd[i].revents & POLLIN && */pollFd[i].fd == socketFd) // TODO test with flaqs in comment
				{
					newConnection();
					// if (pollFd[i].fd == socketFd) // new connection
					// else // data from an other client
				}
				else/* if (pollFd[i].revents & POLLIN)*/ // TODO test with uncomment if condition
					handleClientData(i);
			
			}
			// check all events new connection or data give for one client // TODO
		}
	}
}

Channel* Server::findChannel(const std::string name)
{
 	std::cout << "Chanel name: '" << name << "'" << std::endl;
	std::cout << "channels size: " << channels.size() << std::endl;
	// std::cout << "channels addr: " << &channels << std::endl;
	
	std::map<std::string, Channel*>::iterator it = channels.find(name);
	if (it != channels.end())
		return it->second;
	return (NULL);
}

void Server::addChannel(const std::string name, Channel* channel)
{
	channels[name] = channel;
	// std::map<std::string, Channel*>::iterator it;
	// it = channels.begin();
	// while (it != channels.end())
	// {
	// 	std::cout << "channel list: " << it->first << std::endl;
	// 	it++;
	// }
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
