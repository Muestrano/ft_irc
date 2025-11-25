#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "include.hpp"

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
	Server*	server;

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

	void	test(Client* client, std::string buffer);
	// void	pass_serv(Client* client, std::string buffer);
	// void	pass_chan(Client* client, std::string buffer);
	void	nick(Client* client, std::string buffer);
	// void	user(Client* client, std::string buffer);
	// void	join(Client* client, std::string buffer);
	// void	mode(Client* client, std::string buffer);
	// void	topic(Client* client, std::string buffer);
	// void	invite(Client* client, std::string buffer);
	// void	kick(Client* client, std::string buffer);
	// void	privmsg(Client* client, std::string buffer);
	// void	exit(Client* client, std::string buffer);
	// void	quit(Client* client, std::string buffer);

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
