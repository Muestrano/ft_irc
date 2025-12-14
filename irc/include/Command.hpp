#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "include.hpp"

enum ErrorCode 
{
	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHCHANNEL = 403,
	ERR_CANNOTSENDTOCHAN = 404,
	ERR_UNKNOWNCOMMAND = 421,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_USERNOTINCHANNEL = 441,
	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,
	ERR_NOTREGISTERED = 451,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_PASSWDMISMATCH = 464,
	ERR_CHANNELISFULL = 471,
	ERR_UNKNOWNMODE = 472,
	ERR_INVITEONLYCHAN = 473,
	ERR_BADCHANNELKEY = 475,
	ERR_BADCHANMASK = 476,
	ERR_CHANOPRIVSNEEDED = 482
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
		void sendErrorCode(Client* client, ErrorCode errorCode, std::string errorMsg);
		void pass_serv(Client* client, std::string buffer);
		void nick(Client* client, std::string buffer);
		void user(Client* client, std::string buffer);
		void join(Client* client, std::string buffer);
		void who(Client* client, std::string buffer);
		void privmsg(Client* client, std::string buffer);
		void sendWelcome(Client* client);
		void sendMOTD(Client* client);
		void part(Client* client, std::string buffer);
		void kick(Client* client, std::string buffer);
		void displayCurrentModes(Client* client, Channel* channel, const std::string& channelName);
		bool validateModePermissions(Client* client, Channel* channel, const std::string& channelName);
		void broadcastModeChanges(Client* client, Channel* channel, const std::string& channelName, const std::string& appliedModes, const std::string& modeArgs);
		void mode(Client* client, std::string buffer);
		void quit(Client* client, std::string buffer);
		void invite(Client* client, std::string buffer);
		void topic(Client* client, std::string buffer);
};

#endif
