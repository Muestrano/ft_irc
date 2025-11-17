#include "../include/include.hpp"
#include "../include/Server.hpp"


/**
 * @brief Take two arg ./ircserv <port> <password>
*/
int main(int ac, char **av)
{
	(void)ac;
	// parse arg function
	class Server server;
	server.initServer(av[1], av[2]);
	return 0;
}

/*
1. Infra
	a. Server initialisation (socket creation ...)
	b. main loop creation with poll() or similare function
test and run the serv (try with netcat => cf inception)

2. Handling client and parsing
	a. dev client part part to received data
	b. parsing, extract command and include \r\n
	c. Associate command with fonction (std::map) => cf other project
test client authentification

3. Channel and message
	A. add, remove client display msg to all client
	b. Implement channel (JOIN, PART, PRIVMSG)
Try to send msg with hexchat

4. Operator and Mode
	a. Channel operators (KICK, INVITE, TOPIC)
	b. MODE command (with flags)
		i, t, k, o, l => cf subject

5. Test as FUUUUCCCKK


*/