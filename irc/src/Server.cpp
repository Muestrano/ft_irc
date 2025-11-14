#include "../include/Server.hpp"
#include "../include/include.hpp"

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