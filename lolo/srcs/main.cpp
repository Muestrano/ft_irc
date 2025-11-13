#include "../inc/irc.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"

/*	Main
**	@param ac : the number of arguments
**	@param av : the arguments
**	Launch the server
*/

int	main(int ac, char **av)
{
	std::string args;

	if (ac != 3)
		return (printf("Error\n please follow this instructions\n ./ircserv <port> <password>.\n"), 1);
	args = av[1];
	if (args.length() != 4)
		return (printf("Error\n the port must be valid\n"), 1);
	for (int i = 0; args[i]; i++)
		if (!isdigit(args[i]))
			return (printf("Error\n port must be a integer\n"), 1);
	ft_launch_server(av);
	return (0);
}

/*	ft_launch_server
**	@param av : the arguments
**	Launch the server
*/

void	ft_launch_server(char **av)
{
	Server server(av);
	server.Init();
	server.Launch();
}
