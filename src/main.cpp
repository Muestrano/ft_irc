#include "../include/include.hpp"
#include "../include/Server.hpp"


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
	Server server(port, av[2]);
	server.initServer();
	server.startServer();

	return 0;
}
