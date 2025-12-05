#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "include.hpp"

enum ErrorCode 
{
	ERR_NOSUCHCHANNEL = 403,
	ERR_CANNOTSENDTOCHAN = 404,
	ERR_UNKNOWNCOMMAND = 421,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_TARGETLEO = 436,
	ERR_TARGETESTEBAN = 437,
	ERR_NOTONCHANNEL = 442,
	ERR_NOTREGISTERED = 451,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_PASSWDMISMATCH = 464,
	ERR_CHANNELISFULL = 471,
	ERR_INVITEONLYCHAN = 473,
	ERR_BADCHANNELKEY = 475
};

class Client;
class Server;
class Channel;

class Command
{
	private :

		typedef void (Command::*FtCommand)(Client* client, std::string buffer);
		std::map<std::string, FtCommand> CommandMap;
		Server*	server;

	public :

		// Coplien form

		Command();
		Command(const Command& c);
		~Command();
		Command& operator=(const Command&);

		// Setters

		void setServer(Server *server);

		// Public methods

		void set_map(void);
		void prepareCommand(Client* client, std::string line);
		void extractCompleteCommand(Client* client);
		void sendError(Client* client, int codeError, const std::string& message);
		void sendErrorCode(Client* client, ErrorCode errorCode, std::string errorMsg);
		void pass_serv(Client* client, std::string buffer);
		void nick(Client* client, std::string buffer);
		void user(Client* client, std::string buffer);
		void join(Client* client, std::string buffer);
		void mode(Client* client, std::string buffer);
		void who(Client* client, std::string buffer);
		void privmsg(Client* client, std::string buffer);
		void part(Client* client, std::string buffer);
		
		void test(Client* client, std::string buffer); // TODO Delete later.
		//std::string codeToString(int value); // TODO Delete.
		
		// void	pass_chan(Client* client, std::string buffer);
		// void	join(Client* client, std::string buffer);
		// void	topic(Client* client, std::string buffer);
		// void	invite(Client* client, std::string buffer);
		// void	kick(Client* client, std::string buffer);
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
