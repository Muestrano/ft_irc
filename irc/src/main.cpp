#include "../include/include.hpp"
#include "../include/Server.hpp"


/**
 * @brief Take two arg ./ircserv <port> <password>
*/
int main(int ac, char **av)
{
	std::string	args;
	int	port;

	if (ac < 3)
	{
		std::cerr << "Wrong number of arguments. Please use this format :\n\t./ircserv <port> <password>" << std::endl;
		return(-1);
	}
	if (ac > 3)
	{
		std::cerr << "It really seems that you want to break our code. Don't worry, the parsing is perfect ! 😎" << std::endl;
		return(-1);
	}
	args = av[1];
	if (av[1][0] == '-')
	{
		for (int i = 1; args[i]; i++)
			if (!isdigit(args[i]))
			{
				std::cerr << "Wrong port number. Please use only digits. This is a port NUMBER. N-U-M-B-E-R ! My god." << std::endl;
				return (-1);
			}
		std::cerr << "Why are you being so negative ? Keep calm and be positive with your port number !" << std::endl;
		return (-1);
	}
	if (args.length() > 5)
	{
		std::cerr << "Wrong port number. Please use at max five digits with a port number between 0 and 65535." << std::endl;
		return(-1);
	}
	for (int i = 0; args[i]; i++)
		if (!isdigit(args[i]))
		{
			std::cerr << "Wrong port number. Please use only digits. This is a port NUMBER. N-U-M-B-E-R ! My god." << std::endl;
			return (-1);
		}
	port = atoi(av[1]);
	if (port > 65535)
	{
		std::cerr << "You're seeing it big. Big things, big numbers. But calm down, don't reach over 65535." << std::endl;
		return (-1);
	}
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