#include "../../inc/Server.hpp"

// Bool to check if the server is up
bool ServerUp = true;

/*	Constructor
**	@param av : the arguments of the server
**	Set all commands for my map
*/

Server::Server(char **av)
{
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	_opt = 1;
	_port = atoi(av[1]);
	_password = av[2];

	commandFunctions["PASS"] = &Server::ft_verif_pass;
	commandFunctions["NICK"] = &Server::ft_nick_receive;
	commandFunctions["USER"] = &Server::ft_user_receive;
	commandFunctions["QUIT"] = &Server::ft_quit_user;
	commandFunctions["JOIN"] = &Server::ft_join_receive;
	commandFunctions["MODE"] = &Server::ft_mode_receive;
	commandFunctions["TOPIC"] = &Server::ft_topic_receive;
	commandFunctions["INVITE"] = &Server::ft_invite_receive;
	commandFunctions["PRIVMSG"] = &Server::ft_privmsg;
	commandFunctions["KICK"] = &Server::ft_kick_receive;
  	commandFunctions["PART"] = &Server::ft_part_receive;
}

/*	Destructor
**	Close the socket
*/

Server::~Server()
{
}

/*	ft_sigint
**	@param sig : the signal
**	Handle the signal SIGINT
** 	Set the server to false
*/

void	ft_sigint(int sig)
{
	if (sig == SIGINT)
	{
		std::cout << "Server is shutting down" << std::endl;
		ServerUp = false;
	}
}

/*	Launch
**	Launch the server
**	Accept the clients and read the buffer
**  Manage more than one client
*/

void	Server::Launch()
{
	struct pollfd pstruct[MAX_CLIENT + 1] = {};
	pstruct[0].fd = _socket;
	pstruct[0].events = POLLIN | POLLPRI;
	int	nb_client = 0;

	struct sigaction act;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	act.sa_handler = &ft_sigint;
	sigaction(SIGINT, &act, NULL);

	while (ServerUp)
	{
		int rc = poll(pstruct, nb_client + 1, 5000);
		if (rc > 0)
		{
			if (pstruct[0].revents & POLLIN)
			{
                struct sockaddr_in cliaddr;
				socklen_t addrlen = sizeof(cliaddr);
				_new_socket = accept(_socket, (struct sockaddr *)&cliaddr, &addrlen);
				for (int i = 1; i <= MAX_CLIENT; i++)
				{
					if (pstruct[i].fd == 0)
					{
						pstruct[i].fd = _new_socket;
						pstruct[i].events = POLLIN | POLLPRI;
						nb_client++;
						break;
					}
				}
			}
		}
		for (int i = 1; i <= MAX_CLIENT; i++)
		{
			if (pstruct[i].fd > 0 && pstruct[i].revents & POLLIN)
			{
				char buffer[1024] = { 0 };
				_valread = read(pstruct[i].fd, buffer, 1024 - 1);
				if (_valread == -1 || _valread == 0)
				{
					pstruct[i].fd = 0;
					pstruct[i].events = 0;
					pstruct[i].revents = 0;
					nb_client--;
				}
				else
					ft_parse_buffer(buffer, pstruct[i].fd);
			}
		}
	}
}

/*	Init
**	Init the server
**	Bind the socket and listen
*/

void	Server::Init()
{
	struct sockaddr_in address;

	if (_socket == 0)
	{
		std::cerr << "socket failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_opt, sizeof(_opt)))
	{
		std::cerr << "setsockopt failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);
	if (bind(_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cerr << "bind failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Server is running on port " << _port << std::endl;
	if (listen(_socket, 3) < 0)
	{
		std::cerr << "listen failed" << std::endl;
		exit(EXIT_FAILURE);
	}
}
