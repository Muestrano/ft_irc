#include "../../inc/Server.hpp"

/*	ft_parse_buffer
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Parse the buffer and call the right function to handle the command
*/

void	Server::ft_parse_buffer(std::string buffer, int client)
{
	ft_getServerName();
	if (DEBUG) // Just print the buffer and fd of client for debug
		std::cout << "[FD] --> " << client << " | " << "Buffer :" << buffer << std::endl;

	std::istringstream iss(buffer);
	std::string command;
	iss >> command;

    std::map<std::string, CommandFunction>::iterator it = commandFunctions.find(command);
	if (it != commandFunctions.end())
		(this->*(it->second))(buffer, client);
}

/*	ft_verif_pass
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Verify the password of the client
*/

void	Server::ft_verif_pass(std::string buffer, int client)
{
	std::string pass;
	if (ft_verif_empty(buffer, "PASS :", client))
		return ;
	pass = buffer.substr(6, buffer.length() - 7);
	if (pass.compare(0, _password.length() + 1, _password) == 0)
	{
		if (findFd(client) != -1)
		{
			ft_send_error(client, 462, "PASS", "ERR_ALREADYREGISTRED");
			return ;
		}
		else
			_client.insert(std::pair<int, Client>(client , Client(client)));
	}
	else if (pass.length() == 0)
	{
		ft_send_error(client, 461, "PASS", "ERR_NEEDMOREPARAMS");
		return ;
	}
	else
	{
		ft_send_error(client, 464, "PASS", "ERR_PASSWDMISMATCH");
		return ;
	}
}

/*	ft_nick_receive
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Receive the nickname of the client
*/

void	Server::ft_nick_receive(std::string buffer, int client)
{
	std::string	nick;
	Client	*user;

	if (ft_verif_empty(buffer, "NICK ", client))
		return ;
	if (findFd(client) == -1)
	{
		ft_send_error(client, 464, "PASS", "ERR_PASSWDMISMATCH");
		return ;
	}
	nick = buffer.substr(5, buffer.length() - 6);
	if (buffer.length() <= 6)
	{
		ft_send_error(client, 431, "NICK", "ERR_NONICKNAMEGIVEN");
		return ;
	}
	if (nick.find_first_of("*:@,!? ", 0) != std::string::npos)
	{
		ft_send_error(client, 432, "NICK", "ERR_ERRONEUSNICKNAME");
		return ;
	}
	if (findFdByNickname(nick) != -1)
	{
		ft_send_error(client, 433, "NICK", "ERR_NICKNAMEINUSE");
		return ;
	}
	else
	{
		user = findClient(client);
		user->setNickname(nick);
	}
}

/*	ft_user_receive
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Receive the username of the client
*/

void	Server::ft_user_receive(std::string buffer, int client)
{
	std::string	username;
	Client	*user;

	if (ft_verif_empty(buffer, "USER ", client))
		return ;
	if (ft_verif_user(client) == 1)
		return ;
	username = buffer.substr(5, buffer.length() - 6);
	if (username.find_first_of(" ", 0) != std::string::npos)
		username.erase(username.find_first_of(" ", 0), username.length());
	if(findFd(client))
	{
		user = findClient(client);
		if (!user->getUsername().empty())
			return ;
		user->setUsername(username);
	}
	ft_welcome(client);
}

/*	ft_quit_user
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Quit the user from the server
*/

void	Server::ft_quit_user(std::string buffer, int client)
{
	std::string	message;
	Client		*user;

	if (ft_verif_empty(buffer, "QUIT :", client))
		return ;
	if (ft_verif_user(client) == 1)
		return ;
	if (findClient(client)->getNickname() == "" || findClient(client)->getUsername() == "")
    {
        ft_send_error(client, 451, "ERROR", "ERR_NOTREGISTERED");
        return ;
    }
	if (buffer.find(":", 0) != std::string::npos)
		message = buffer.substr(buffer.find(":", 0) + 1, buffer.length() - buffer.find(":", 0) - 2);
	else
		message = "Client Quit";
	user = findClient(client);
	if (user)
	{
		std::map<std::string, Channel>::iterator it;
		for (it = _channel.begin(); it != _channel.end(); ++it)
			it->second.quitChannel(user, message);
		// Check if channel is empty and delete it
		_client.erase(client);
	}
	else
	{
		ft_send_error(client, 451, "QUIT", "ERR_NOTREGISTERED");
		return ;
	}
}

/*	ft_join_receive
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Join the client to a channel
*/

void	Server::ft_join_receive(std::string buffer, int client)
{
	std::string channel;
	std::string password;
	Channel	*chan;

	if (ft_verif_empty(buffer, "JOIN ", client))
		return ;
	if (ft_verif_user(client) == 1)
		return ;
	if (findClient(client)->getNickname() == "" || findClient(client)->getUsername() == "")
    {
        ft_send_error(client, 451, "ERROR", "ERR_NOTREGISTERED");
        return ;
    }
	if (buffer.find("#", 0) != std::string::npos)
	{
		if (buffer.find(" ", 5) != std::string::npos)
		{
			buffer.erase(0, buffer.find("#", 0) + 1);
			channel = buffer.substr(0, buffer.find(" ", 0));
			buffer.erase(0, buffer.find(" ", 0) + 1);
			password = buffer.substr(0, buffer.length() - 1);
			chan = findChannel(channel);
			if (!chan)
				_channel.insert(std::pair<std::string, Channel>(channel , Channel(channel, password, findClient(client))));
			else
				chan->addUser(findClient(client), password);
		}
		else
		{
			channel = buffer.substr(6, buffer.length() - 7);
			chan = findChannel(channel);
			if (!chan)
				_channel.insert(std::pair<std::string, Channel>(channel , Channel(channel, findClient(client))));
			else
				chan->addUser(findClient(client), "");
		}
	}
}

/*	ft_topic_receive
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Change the topic of the channel
*/

void	Server::ft_topic_receive(std::string buffer, int client)
{
	std::string	channel;
	std::string	newTopic;
	Channel	*chan;

	if (ft_verif_empty(buffer, "TOPIC ", client))
		return ;
	if (ft_verif_user(client) == 1)
		return ;
	if (findClient(client)->getNickname() == "" || findClient(client)->getUsername() == "")
    {
        ft_send_error(client, 451, "ERROR", "ERR_NOTREGISTERED");
        return ;
    }
	if (buffer.find("#", 0) == std::string::npos)
	{
		ft_send_error(client, 461, "TOPIC", "ERR_NEEDMOREPARAMS");
		return ;
	}
	if (buffer.find(":", 0) != std::string::npos)
	{
		buffer.erase(0, buffer.find("#", 0));
		channel = buffer.substr(1, buffer.find(" ", 0) - 1);
		buffer.erase(0, buffer.find(":", 0) + 1);
		buffer.erase(buffer.end() - 1);
		newTopic = buffer;
	}
	else
		channel = buffer.substr(7, buffer.length() - 8);
	chan = findChannel(channel);
	if (chan == NULL)
	{
		ft_send_error(client, 403, "TOPIC", "ERR_NOSUCHCHANNEL");
		return ;
	}
	chan->topic(findClient(client), newTopic);
}

/*	ft_invite_receive
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Invite a user to a channel
*/

void Server::ft_invite_receive(std::string buffer, int client)
{
	std::string	channel;
	std::string	user;
	Channel	*chan;
	Client	*newUser;

	if (ft_verif_empty(buffer, "INVITE ", client))
		return ;
	if (ft_verif_user(client) == 1)
		return ;
	if (buffer.find(" ", 0) == std::string::npos || buffer.find("#", 0) == std::string::npos || buffer.find(" ", 7) == std::string::npos)
	{
		ft_send_error(client, 461, "INVITE", "ERR_NEEDMOREPARAMS");
		return ;
	}
	channel = buffer.substr(buffer.find("#", 0) + 1, buffer.find("\n", buffer.find("#", 0)) - (buffer.find("#", 0) + 1));
	user = buffer.substr(7, buffer.find(" ",7) - 7);
	chan = findChannel(channel);
	newUser = findClient(findFdByNickname(user));
	if (chan && newUser)
		chan->invite(findClient(client), newUser);
	else
	{
		ft_send_error(client, 401, "INVITE", "ERR_NOSUCHNICK");
		return ;
	}
}

/*	ft_mode_receive
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Change the mode of the client
*/

void Server::ft_mode_receive(std::string buffer, int client)
{
	std::string 	mode;
	size_t			args;
	std::string		channel;
	Channel			*chan;

	if (ft_verif_empty(buffer, "MODE ", client))
		return ;
	if (ft_verif_user(client) == 1)
		return ;
	buffer.erase(0, buffer.find_first_of(" ", 0) + 1);
	if (buffer.empty() || buffer.find("#", 0) == std::string::npos)
	{
		ft_send_error(client, 461, "MODE", "ERR_NEEDMOREPARAMS");
		return ;
	}
	if (buffer.find_first_of(" ", 0) == std::string::npos)
		return ;
	channel = buffer.substr(1,buffer.find_first_of(" ", 0) - 1);
	chan = findChannel(channel);
	if (chan == NULL)
	{
		ft_send_error(client, 403, "MODE", "ERR_NOSUCHCHANNEL");
		return ;
	}
	if (chan->clientInChannel(findClient(client)) == 0)
	{
		ft_send_error(client, 441, "MODE", "ERR_USERNOTINCHANNEL");
		return ;
	}
	buffer.erase(0, buffer.find_first_of(" ", 0) + 1);
	if (chan->clientInChannel(findClient(client)) != 2 && buffer != "+b")
	{
		ft_send_error(client, 482, "MODE", "ERR_CHANOPRIVSNEEDED");
		return ;
	}
	args = ft_count_args(buffer);
	buffer.erase(buffer.end() - 1);
	if (args == 0)
	{
		chan->checkMode(&buffer);
	}
	else
	{
		std::string *param = new std::string[args + 2];
		for (size_t i = 0; i <= args; i++)
		{
			param[i] = buffer.substr(0, buffer.find(" ", 0));
			buffer.erase(0, buffer.find(" ", 0) + 1);
		}
		param[args + 1] = "";
		chan->checkMode(param);
		delete [] param;
	}
}

/*	ft_kick_receive
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Kick a user from a channel
*/

void	Server::ft_kick_receive(std::string buffer, int client)
{
	std::string	channel;
	std::string	user;
	Channel	*chan;

	if (ft_verif_empty(buffer, "KICK ", client))
		return ;
	if (ft_verif_user(client) == 1)
		return ;
	if (findClient(client)->getNickname() == "" || findClient(client)->getUsername() == "")
    {
        ft_send_error(client, 451, "ERROR", "ERR_NOTREGISTERED");
        return ;
    }
	if (buffer.find("#", 0) == std::string::npos)
	{
		ft_send_error(client, 461, "KICK", "ERR_NEEDMOREPARAMS");
		return ;
	}
	if (buffer.find(":", 0) != std::string::npos)
	{
		channel = buffer.substr(6, buffer.find(" ", 5) - 6);
		user = buffer.substr(buffer.find(" ", 5) + 1, buffer.find(" ", buffer.find(" ", 5) + 1) - buffer.find(" ", 5) - 1);
	}
	else
	{
		channel = buffer.substr(6, buffer.find(" ", 5) - 6);
		user = buffer.substr(buffer.find(" ", 5) + 1, buffer.length() - buffer.find(" ", 5) - 2);
	}
	chan = findChannel(channel);
	if (chan == NULL)
	{
		ft_send_error(client, 403, "KICK", "ERR_NOSUCHCHANNEL");
		return ;
	}
	chan->kick(findClient(client), user);
}

/*	ft_part_receive
**	@param buffer : the buffer to parse
**	@param client : the fd of the client
**	Quit the client from a channel
*/

void	Server::ft_part_receive(std::string buffer, int client)
{
	std::string	channel;
	Channel	*chan;

	if (ft_verif_empty(buffer, "PART ", client))
		return ;
	if (ft_verif_user(client) == 1)
		return ;
	if (findClient(client)->getNickname() == "" || findClient(client)->getUsername() == "")
    {
        ft_send_error(client, 451, "ERROR", "ERR_NOTREGISTERED");
        return ;
    }
	if (buffer.find("#", 0) == std::string::npos)
	{
		ft_send_error(client, 461, "PART", "ERR_NEEDMOREPARAMS");
		return ;
	}
	if (buffer.find(":", 0) != std::string::npos)
		channel = buffer.substr(6, buffer.find(" ", 5) - 6);
	else
		channel = buffer.substr(6, buffer.length() - 7);
	chan = findChannel(channel);
	if (chan == NULL)
	{
		ft_send_error(client, 403, "PART", "ERR_NOSUCHCHANNEL");
		return ;
	}
	chan->quitChannel(findClient(client), buffer.substr(buffer.find(":", 0) + 1, buffer.length() - buffer.find(":", 0) - 2));
	if (chan->getNBUser() == 0)
		_channel.erase(channel);
}

/*  ft_privmsg
**  Sent a private message to a user or a channel
** @param buffer : the message
** @param client : the client who sent the message
*/

void	Server::ft_privmsg(std::string buffer, int client)
{
    Client *user;
    Channel *chan;
    std::string receiver;
    std::string message;
    std::string channel;

    if (ft_verif_user(client) == 1)
        return ;
    if (findClient(client)->getNickname() == "" || findClient(client)->getUsername() == "")
    {
        ft_send_error(client, 451, "ERROR", "ERR_NOTREGISTERED");
        return ;
    }
    if (buffer.find("#", 0) != std::string::npos)
    {
        channel = buffer.substr(9, buffer.find(" ", 0) - 10);
        if (channel.find_first_of(" ", 0) != std::string::npos)
            channel.erase(channel.find_first_of(" ", 0), channel.length());
    }
	if (buffer.find(":", 0) != std::string::npos)
    	message = std::string(buffer.begin() + buffer.find(":", 0) + 1, buffer.end());
	else
	{
		ft_send_error(client, 461, "PASS", "ERR_NEEDMOREPARAMS");
		return ;
	}
    receiver = buffer.substr(8, buffer.length() - 9);
    if (receiver.find_first_of(" ", 0) != std::string::npos)
		receiver.erase(receiver.find_first_of(" ", 0), receiver .length());
    user = findClient(client);
    if (channel != "")
    {
        if (findChannelByName(channel) == -1)
		{
            ft_send_error(client ,401, "ERROR", "ERR_NOSUCHCHANNEL");
			return ;
		}
		chan = findChannel(channel);
		if (chan->clientInChannel(user) == 0)
		{
            ft_send_error(client ,404, "ERROR", "ERR_CANNOTSENDTOCHAN");
			return ;
		}
        else
            SendMessageToChannel(channel, user, message);
    }
    else
    {
        if (findFdByNickname(receiver) == -1)
        {
            ft_send_error(client ,401, "ERROR", "ERR_NOSUCHNICK");
            return ;
        }
        else
            SendMessage(findFdByNickname(receiver), user->getNickname(), message);
    }
}
