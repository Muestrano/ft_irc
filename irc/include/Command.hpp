#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "include.hpp"

#define ERR_UNKNOWNCOMMAND "421"
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// What do you whant pierre, cammand associate directly by Server class or non instanciable class funtion with 
	// command function to access in another cpp file ?
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


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

	typedef void (Command::*FtCommand)(std::string, int);
	std::map<std::string, FtCommand> CommandMap;

	public :

	//Coplien form
	Command();
	Command(const Command&);
	~Command();
	Command& operator=(const Command&);

	//Methods to handle commands

	void extractCompleteCommand(Client* client, Server* server);
	void prepareCommand(Client* client, Server* server, std::string line);
	void sendError(Client* client, int codeError, const std::string& message);
	std::string codeToString(int value);
	void	set_map(void);

	// void	ft_pass_serv(std::string buffer, int client); ???????
	void	ft_test(Client* client, Server* server, std::string buffer);
	void	ft_pass_chan(std::string buffer, int client);
	void	ft_nick(std::string buffer, int client);
	void	ft_user(std::string buffer, int client);
	void	ft_join(std::string buffer, int client);
	void	ft_mode(std::string buffer, int client);
	void	ft_topic(std::string buffer, int client);
	void	ft_invite(std::string buffer, int client);
	void	ft_kick(std::string buffer, int client);
	void	ft_privmsg(std::string buffer, int client);
	void	ft_exit(std::string buffer, int client);
	void	ft_quit(std::string buffer, int client);


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