#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "include.hpp"

#define ERR_UNKNOWNCOMMAND "421"
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// What do you whant pierre, cammand associate directly by Server class or non instanciable class funtion with 
	// command static function to access in another cpp file ?
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/*
Forme canonique :
        T (); // Constructeur par défaut
        T (const T&); // Constructeur de recopie
        ~T (); // Destructeur éventuellement virtuel
        T &operator=(const T&); // Operator d'affectation
*/
// class Client;
// class Server;

class Server;
class Client;

class Command
{
	private :
	//Coplien form
	Command();
	Command(const Command&);
	~Command();
	Command& operator=(const Command&);

	// typedef void (Command::*FtCommand)(std::string, int);
	// std::map<std::string, FtCommand> CommandMap;
	typedef void (*FtCommand)(Client* client, Server* server, std::string buffer);
    static std::map<std::string, FtCommand> CommandMap;

	public :


	//Methods to handle commands
	void	ft_redirect_buffer(std::string buffer, int client);
	static void	set_map(void);

	static void extractCompleteCommand(Client* client, Server* server); //why static ??
	static void prepareCommand(Client* client, Server* server, std::string line);
	static void sendError(Client* client, int codeError, const std::string& message);
	static std::string codeToString(int value);

	// static void executeCommand(Server* server, Client* client, const std::string& line);

	// void	ft_pass_serv(std::string buffer, int client); ???????
	static void	ft_pass_chan(Client* client, Server* server, std::string buffer);
	static void	ft_nick(Client* client, Server* server, std::string buffer);
	static void	ft_user(Client* client, Server* server, std::string buffer);
	static void	ft_join(Client* client, Server* server, std::string buffer);
	static void	ft_mode(Client* client, Server* server, std::string buffer);
	static void	ft_topic(Client* client, Server* server, std::string buffer);
	static void	ft_invite(Client* client, Server* server, std::string buffer);
	static void	ft_kick(Client* client, Server* server, std::string buffer);
	static void	ft_privmsg(Client* client, Server* server, std::string buffer);
	static void	ft_exit(Client* client, Server* server, std::string buffer);
	static void	ft_quit(Client* client, Server* server, std::string buffer);

/*
	/PASS <password>\
	/NICK <nickname>\
	/USER <username> <hostname> <servername> <realname>\
	/JOIN <#channel> '<pass>'\
	/MODE <#channel> <mode> [OPERATOR]\
	/TOPIC <#channel> <topic> [OPERATOR]\
	/INVITE <nickname> <#channel>\
	/KICK <#channel> <nickname> [OPERATOR]\
	/PRIVMSG <nickname> <message>\
	/PART <#channel>\
	/QUIT <message>\n";
*/
};

#endif