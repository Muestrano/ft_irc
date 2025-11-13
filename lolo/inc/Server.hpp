#pragma once

#define MAX_CLIENT 20
#define DEBUG false

#include "irc.hpp"
#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <ctime>
#include "Client.hpp"
#include "Channel.hpp"

class Client;
class Channel;

class Server
{
	public:
		Server(char **av);
		~Server();

		/* Main */
		void	Init();
		void	Launch();

		/* Parsing and Commands */
		void	ft_parse_buffer(std::string buffer, int client);
		void	ft_verif_pass(std::string buffer, int client);
		void	ft_nick_receive(std::string buffer, int client);
		void	ft_user_receive(std::string buffer, int client);
		void	ft_quit_user(std::string buffer, int client);
		void	ft_join_receive(std::string buffer, int client);
		void	ft_mode_receive(std::string buffer, int client);
		void	ft_topic_receive(std::string buffer, int client);
		void	ft_invite_receive(std::string buffer, int client);
		void	ft_kick_receive(std::string buffer, int client);
		void	ft_privmsg(std::string buffer, int client);
		void	ft_part_receive(std::string buffer, int client);
		std::string ft_ParseChannel(std::string buffer);
		std::string ft_ParseClient(std::string buffer);
		/* Utils */
		Channel 	*findChannel(std::string name);
		Client 		*findClient(int fd);
		std::string ft_getServerName();
		int 		findFdByNickname(const std::string& nickname);
		int 		findFd(int fd);
		int 		findChannelByName(const std::string& name);
		void		ft_welcome(int fd);
		int 		ft_verif_empty(std::string buffer, std::string cmd, int client);
		int 		ft_count_args(std::string buffer);
		int 		ft_verif_user(int client);
		/* Sender */
		void		ft_send_error(int fd, int error, std::string command, std::string type);
		void		SendMessage(int fd, const std::string& sender, const std::string& message);
		void 		SendMessageToChannel(const std::string& channel, Client* sender, const std::string& message);
	private:
		int		_port;
		int 	_opt;
		int		_socket;
		int		_new_socket;
		ssize_t		_valread;
		std::string	_password;
		std::string	_servername;
		typedef void (Server::*CommandFunction)(std::string, int);
		std::map<std::string, CommandFunction> commandFunctions;
		std::map<std::string, Channel> _channel;
		std::map<int , Client> _client;
};
