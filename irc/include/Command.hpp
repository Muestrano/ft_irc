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

	typedef void (Command::*FtCommand)(Client* client, std::string buffer);
    std::map<std::string, FtCommand> CommandMap;

	public :

	//Coplien form
	
	Command();
	Command(const Command&);
	~Command();
	Command& operator=(const Command&);

	//Methods to handle commands

	void extractCompleteCommand(Client* client);
	void prepareCommand(Client* client, std::string line);
	void sendError(Client* client, int codeError, const std::string& message);
	std::string codeToString(int value);
	void	set_map(void);

	// void	ft_pass_serv(std::string buffer, int client); ???????
	void	ft_test(Client* client, std::string buffer);
	// void	ft_pass_chan(Client* client, std::string buffer);
	// void	ft_nick(Client* client, std::string buffer);
	// void	ft_user(Client* client, std::string buffer);
	// void	ft_join(Client* client, std::string buffer);
	// void	ft_mode(Client* client, std::string buffer);
	// void	ft_topic(Client* client, std::string buffer);
	// void	ft_invite(Client* client, std::string buffer);
	// void	ft_kick(Client* client, std::string buffer);
	// void	ft_privmsg(Client* client, std::string buffer);
	// void	ft_exit(Client* client, std::string buffer);
	// void	ft_quit(Client* client, std::string buffer);

/*
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
*/
};

#endif