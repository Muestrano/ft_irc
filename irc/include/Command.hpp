#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "include.hpp"

#define ERR_UNKNOWNCOMMAND "421"

/*
Forme canonique :
        T (); // Constructeur par défaut
        T (const T&); // Constructeur de recopie
        ~T (); // Destructeur éventuellement virtuel
        T &operator=(const T&); // Operator d'affectation
*/

class Client;
class Server;

class Command
{
	private :

	typedef void (Command::*FtCommand)(Client* client, Server* server, std::string buffer);
    std::map<std::string, FtCommand> CommandMap;

	public :

	//Coplien form
	Command();
	Command(const Command&);
	~Command();
	Command& operator=(const Command&);

	//Methods to handle commands
	void	set_map(void);
	void extractCompleteCommand(Client* client, Server* server);
	void prepareCommand(Client* client, Server* server, std::string line);
	void sendError(Client* client, int codeError, const std::string& message);
	std::string codeToString(int value);
	void	set_map(void);

	// void	ft_pass_serv(std::string buffer, int client); ???????
	void	ft_test(Client* client, Server* server, std::string buffer);
	// void	ft_pass_chan(Client* client, Server* server, std::string buffer);
	// void	ft_nick(Client* client, Server* server, std::string buffer);
	// void	ft_user(Client* client, Server* server, std::string buffer);
	// void	ft_join(Client* client, Server* server, std::string buffer);
	// void	ft_mode(Client* client, Server* server, std::string buffer);
	// void	ft_topic(Client* client, Server* server, std::string buffer);
	// void	ft_invite(Client* client, Server* server, std::string buffer);
	// void	ft_kick(Client* client, Server* server, std::string buffer);
	// void	ft_privmsg(Client* client, Server* server, std::string buffer);
	// void	ft_exit(Client* client, Server* server, std::string buffer);
	// void	ft_quit(Client* client, Server* server, std::string buffer);


	// /PASS <password>\
	// /NICK <nickname>\
	// /USER <username> <hostname> <servername> <realname>\
	// /JOIN <#channel> '<pass>'\
	// /MODE <#channel> <mode> [OPERATOR]\
	// /TOPIC <#channel> <topic> [OPERATOR]\
	// /INVITE <nickname> <#channel>\
	// /KICK <#channel> <nickname> [OPERATOR]\
	// /PRIVMSG <nickname> <message>\
	// /PART <#channel>\
	// /QUIT <message>\n";
};

#endif