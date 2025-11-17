#include "../include/Server.hpp"
#include "../include/include.hpp"


void Server::initServer(std::string port, std::string password)
{
	this->port = atoi(port.c_str());
	this->password = password; // need to cast it to have const password

}
/**
 * @param serverAddre socketaddr_in struct contain the server adress
 * @param AF_INET ipv4 protocol
 * @param SOCK_STREAM TCP socket
 * @param htons convert port to network byte ordre
 * @param INADDR_ANY accept all ip conncetion
*/
void Server::startServer()
{
	this->socketFd = socket(AF_INET, SOCK_STREAM, 0); //
	this->serverAddr.sin_family = AF_INET;
	this->serverAddr.sin_port = htons(8080);
	this->serverAddr.sin_addr.s_addr = INADDR_ANY;

	bind(this->socketFd, (struct sockaddr*)&this->serverAddr, sizeof(this->serverAddr));
	listen(this->socketFd, 1);

	int clientSocket = accept(this->socketFd, NULL, NULL);

	char buffer[1024] = {0};
	recv(socketFd, buffer, sizeof(buffer), 0);
	std::cout << "Message from client: " << buffer << std::endl;
	close(socketFd);

/*
poll give info if the operand accept, recv, send can execute
fcntl with O_NONBLOCK flag to handle A/I operation
POLLIN
POLLOUT
POLLERR

	LOOP:
		-init pollfd struc with socket (client and listening)
		- call poll()
		- check all socket to see who's waiting for operation (send, recv...)
		create tab or vector for pollfd ?
*/
	
}
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