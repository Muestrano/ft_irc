#include "../../inc/Server.hpp"

/*	ft_send_error
**	@param fd : the fd of the client
**	@param error : the error code
**	@param command : the command
**	@param type : the type of error
**	Send an error to the client
*/

void	Server::ft_send_error(int fd, int error, std::string command, std::string type)
{
	std::string error_code;
	std::string error_message;
	std::string error_send;

	error_code = SSTR(error);
	error_message = " :" + type;
	error_send = ":" + _servername + " " + error_code + " " + command + error_message + "\r\n";
	send(fd, error_send.c_str(), error_send.length(), 0);
}

/*  SendMessage
**  Sent a message to a user
** @param fd : the file descriptor of the user
** @param sender : the nickname of the sender
** @param message : the message
*/

void Server::SendMessage(int fd, const std::string& sender, const std::string& message)
{
    Client* client = findClient(fd);
    if (client)
    {
        std::string msg = ":" + sender + " PRIVMSG " + client->getNickname() + " :" + message + "\n";
        send(fd, msg.c_str(), msg.length(), 0);
    }
}

/*  SendMessageToChannel
**  Sent a message to a channel
** @param channel : the name of the channel
** @param sender : the client who sent the message
** @param message : the message
*/

void Server::SendMessageToChannel(const std::string& channel, Client* sender, const std::string& message)
{
    std::map<std::string, Channel>::iterator it = _channel.find(channel);
    if (it != _channel.end())
        it->second.SendAllFD(":" + sender->getNickname() + " PRIVMSG #" + channel + " :" + message + "\n", sender->getFd());
}

/*  findChannelByName
**  Find the channel by the name
** @param name : the name of the channel
** @return 1 if the channel is found, -1 if not
*/

int Server::findChannelByName(const std::string& name)
{
    for (std::map<std::string, Channel>::iterator it = _channel.begin(); it != _channel.end(); ++it)
    {
        if (it->first == name)
            return 1;
    }
    return -1;
}

/*  ft_welcome
**  Sent the welcome message to the client
** @param fd : the file descriptor of the client
*/

void Server::ft_welcome(int fd)
{
    std::string welcome = findClient(fd)->getNickname() + " :Welcome to the " + ft_getServerName() + " Network, " + findClient(fd)->getNickname() + "!" + findClient(fd)->getUsername() + "@" + ft_getServerName() + "\n";
	std::string motd =  findClient(fd)->getNickname() + " :Here you have all the commands you can use\n";
	std::string motd2 = " :/PASS <password>\
	\n :/NICK <nickname>\
	\n :/USER <username> <hostname> <servername> <realname>\
	\n :/JOIN <#channel> '<pass>'\
	\n :/MODE <#channel> <mode> [OPERATOR]\
	\n :/TOPIC <#channel> <topic> [OPERATOR]\
	\n :/INVITE <nickname> <#channel>\
	\n :/KICK <#channel> <nickname> [OPERATOR]\
	\n :/PRIVMSG <nickname> <message>\
	\n :/PART <#channel>\
	\n :/QUIT <message>\n";
	send(fd, welcome.c_str(), welcome.length(), 0);
	send(fd, motd.c_str(), motd.length(), 0);
	send(fd, motd2.c_str(), motd2.length(), 0);
}

/*  ft_count_args
**  Count the number of arguments in a string
** @param buffer : the string
** @return the number of arguments
*/

int		Server::ft_count_args(std::string buffer)
{
	int	count(0);

	for (size_t i = 0; i < buffer.length(); ++i) {
			if (buffer[i] == ' ') {
				count++;
			}
    }	return (count);
}

/*  ft_verif_empty
**  Verify if the string is empty
** @param buffer : the string
** @param cmd : the command
** @param client : the file descriptor of the client
** @return 1 if the string is empty, 0 if not
*/

int		Server::ft_verif_empty(std::string buffer, std::string cmd, int client)
{
	if (buffer.compare(0, cmd.length(), cmd))
	{
		ft_send_error(client, 461, cmd, "ERR_NEEDMOREPARAMS");
		return (1);
	}
	return (0);
}

/*  ft_verif_user
**  Verify if the user is registered
** @param client : the file descriptor of the client
** @return 1 if the user is not registered, 0 if not
*/

int Server::ft_verif_user(int client)
{
    if (findFd(client) == -1)
    {
        ft_send_error(client, 451, "ERROR", "ERR_NOTREGISTERED");
        return (1);
    }
    return (0);
}

/*  ft_getServerName
**  Get the server name
** @return the server name
*/

std::string		Server::ft_getServerName()
{
	char hostname[1024];

	if (gethostname(hostname, sizeof(hostname)) == -1)
	{
		std::cerr << "gethostname failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	_servername = hostname;
	return (_servername);
}

/*	FindFdByNickname
**	Find the file descriptor by the nickname of the client
**	@param nickname : the nickname of the client
**	@return the file descriptor
*/

int Server::findFdByNickname(const std::string& nickname)
{
	std::map<int, Client>::iterator it;
	for (it = _client.begin(); it != _client.end(); ++it)
	{
		if (it->second.getNickname() == nickname)
			return (it->second.getFd());
	}
	return (-1);
}

/*	FindFd
**	Find the file descriptor
**	@param fd : the file descriptor
**	@return 1 if the file descriptor is found, -1 if not
*/

int Server::findFd(int fd)
{
	std::map<int, Client>::iterator it;
	for (it = _client.begin(); it != _client.end(); ++it)
	{
		if (it->first == fd)
			return (1);
	}
	return (-1);
}

/*	FindClient
**	Find the client by the file descriptor
**	@param fd : the file descriptor
**	@return the client
*/

Client *Server::findClient(int fd)
{
	std::map<int, Client>::iterator it;
	for (it = _client.begin(); it != _client.end(); ++it)
	{
		if (it->first == fd)
			return (&it->second);
	}
	return (NULL);
}

/*	FindChannel
**	Find the channel by the name
**	@param name : the name of the channel
**	@return the channel
*/

Channel *Server::findChannel(std::string name)
{
	std::map<std::string, Channel>::iterator it;
	for (it = _channel.begin(); it != _channel.end(); ++it)
	{
		if (it->first == name)
			return (&it->second);
	}
	return (NULL);
}
