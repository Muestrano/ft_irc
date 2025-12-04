#include "../include/Server.hpp"
#include "../include/include.hpp"

// Coplien form

Command::Command() : server(NULL)
{
	set_map();
}

Command::Command(const Command& c) : CommandMap(c.CommandMap) 
{
}

Command::~Command()
{
}

Command& Command::operator=(const Command& c)
{
	if (this != &c)
	{
		this->CommandMap = c.CommandMap;
	}
	return (*this);
}

// Setters

void Command::setServer(Server *srv)
{
	this->server = srv;
}

// Methods to handle commands.

/**
 * @brief Sets the CommandMap attribute
*/
void Command::set_map(void)
{
	CommandMap["TEST"] = &Command::test;
	CommandMap["PASS"] = &Command::pass_serv;
	CommandMap["NICK"] = &Command::nick;
	CommandMap["USER"] = &Command::user;
	CommandMap["JOIN"] = &Command::join;
	
	CommandMap["MODE"] = &Command::mode;
	CommandMap["WHO"] = &Command::mode;
	// CommandMap["TOPIC"] = &Command::topic;
	// CommandMap["INVITE"] = &Command::invite;
	// CommandMap["KICK"] = &Command::kick;
	CommandMap["PRIVMSG"] = &Command::privmsg;
	// CommandMap["PART"] = &Command::exit;
	// CommandMap["QUIT"] = &Command::quit;
}

/**
 * @brief Extract the command and params.
 * Performs the appropriate rights check depending on the command called.
 * Call the command from the command map.
 * 
*/
void Command::prepareCommand(Client* client, std::string line)
{
	std::istringstream ss(line);
	std::string command;
	ss >> command;

	std::cout << "line received: '" << line << "'" << std::endl;
	std::transform(command.begin(), command.end(), command.begin(), ::toupper); // upper conversion command

	std::string param;
	std::getline(ss, param);

	param.erase(0, param.find_first_not_of(" ")); //erase space befor params

	// Authentication checking
	if (command != "PASS" && command != "CAP" && command != "QUIT")
	{
		if (!client->getIsAuthenticated())
		{
			sendErrorCode(client, ERR_NOTREGISTERED, command);
			return;
		}
	}

	// Registration checking
	if (command == "JOIN" || command == "PRIVMSG" || command == "MODE" 
		|| command == "WHO" || command == "TOPIC" || command == "INVITE"
		|| command == "KICK" || command == "PART")
	{
		if (!client->getIsRegistered())
		{
			sendErrorCode(client, ERR_NOTREGISTERED, command);
			return;
		}
	}

	if (CommandMap.find(command) != CommandMap.end())
	{
		FtCommand funcMap = CommandMap[command];
		(this->*funcMap)(client, param); 
	}
	else
	{
		sendErrorCode(client, ERR_UNKNOWNCOMMAND, command);
	}
}

/**
 * @brief Extract all command to pass it in the handler command
 * @param buffer Input from the client
 * @param client Client's fd 
*/
void Command::extractCompleteCommand(Client* client)
{
	std::string& buffer = client->getBuffer();

	size_t pos;
	while ((pos = buffer.find("\r\n")) != std::string::npos)
	{
		std::string line = buffer.substr(0, pos);
		buffer.erase(0, pos + 2); // Supp \r\n for the next  command
		prepareCommand(client, line);
	}
	buffer.erase(0, pos + 2);
}

// TEMP - TODO implement in sendErrorCode funct to get rid of it.
// void Command::sendError(Client* client, int codeError, const std::string& command)
// {
// 	std::string nickname = client->getNickName();
// 	if (nickname.empty())
// 		nickname = "*";
	
// 	std::stringstream error;
// 	error << ":localhost " << codeError << " " << nickname;
// 	error << " " << command << " :Unknown command\r\n";
	
// 	std::string stringError = error.str();
// 	send(client->getFd(), stringError.c_str(), stringError.length(), 0);
// }

/**
 * @brief Handles error codes and send the appropriate message
 * @param Client The pointer of the client
 * @param errorCode <ERR_ERROREXAMPLE> the type of error
 * @param errorMsg The string appended at the end of the error message
 */
void Command::sendErrorCode(Client* client, ErrorCode errorCode, std::string errorMsg)
{
	std::string nickname = client->getNickName();
	if (nickname.empty())
		nickname = "*";
	std::stringstream error;
	switch (errorCode)
	{
		case ERR_UNKNOWNCOMMAND: // "<client> <command> :Unknown command"
			error << client->getNickName() << " " << errorMsg << " :Unknown command.";
			break;
		case ERR_NONICKNAMEGIVEN: // "<client> :No nickname given"
			error << client->getNickName() << " :No nickname given.";
			break;
		case ERR_ERRONEUSNICKNAME: // "<client> <nick> :Erroneus nickname"
			error << client->getNickName() << " " << errorMsg << " :Erroneus nickname. The first character of your nickname cannot be \"#\" neither \":\". You can't have space character in your nickname.";
			break;
		case ERR_NICKNAMEINUSE: // "<client> <nick> :Nickname is already in use"
			error << client->getNickName() << " " << errorMsg << " :Nickname is already in use. Please try again with another one.";
			break;
		case ERR_NOTREGISTERED: // "<client> :You have not registered"
			error << client->getNickName() << " :You have not registered"; 
		case ERR_NEEDMOREPARAMS: // "<client> <command> :Not enough parameters"
			error << client->getNickName() << " " << errorMsg << " :Not enough parameters.";
			break;
		case ERR_ALREADYREGISTERED: // "<client> :You may not reregister"
			error << client->getNickName() << " :You may not register.";
			break;
		case ERR_PASSWDMISMATCH: // "<client> :Password incorrect"
			error << client->getNickName() << " :Password incorrect";
			break;
		case ERR_CHANNELISFULL: //"<client> <channel> :Cannot join channel (+l)"
			error << client->getNickName() << " " << errorMsg << " :Cannot join channel (+l)";
			break;
		case ERR_INVITEONLYCHAN: // "<client> <channel> :Cannot join channel (+i)"
			error << client->getNickName() << " " << errorMsg << " :Cannot join channel (+i)";
			break;
		case ERR_BADCHANNELKEY: // "<client> <channel> :Cannot join channel (+k)"
			error << client->getNickName() << " " << errorMsg << " :Cannot join channel (+k)";
			break;
		default:
			break;
	}
	error << "\r\n";
	std::string stringError = error.str();
	send(client->getFd(), stringError.c_str(), stringError.length(), 0);
}

/**
 * @brief Handles the PASS command for client authentication.
 * 
 * Verifies the password provided by the client against the server password.
 * Sends appropriate error codes in cases of errors
 * 
 * @param client Pointer to the Client attempting to authenticate.
 * @param buffer String containing the password provided by the client.
 */
void	Command::pass_serv(Client* client, std::string buffer)
{
	if (client->getIsAuthenticated())
	{
		sendErrorCode(client, ERR_ALREADYREGISTERED, "PASS");
		return;
	}

	if (buffer.empty())
	{
		sendErrorCode(client, ERR_NEEDMOREPARAMS, "PASS");
		return;
	}

	if (buffer == server->getPassword())
	{
		client->setIsAuthenticated(true);
		std::cout << "[AUTH] Client " << client->getFd() << " - Authentication SUCCESS" << std::endl;
		std::string response = ":localhost NOTICE * :Password accepted\r\n"; // TODO Replace \n by std::endl and '*' by good name ?
		send(client->getFd(), response.c_str(), response.length(), 0);
	}
	else
	{
		sendErrorCode(client, ERR_PASSWDMISMATCH, "PASS");
		std::cout << "[AUTH] Client " << client->getFd() << " - Wrong password" << std::endl;
	}
}

/**
 * @brief Handles the /NICK command
 * @param client the pointer of the client
 * @param buffer the parameters of the command
 */
void Command::nick(Client* client, std::string buffer)
{
	if (buffer.empty())
	{
		sendErrorCode(client, ERR_NONICKNAMEGIVEN, "");
		return ;
	}
	else if (this->server->isNickRegistered(buffer))
	{
		sendErrorCode(client, ERR_NICKNAMEINUSE, buffer);
		return ;
	}
	else if (buffer[0] == '#' || buffer[0] == ':' || buffer.find(" ") != std::string::npos)
	{
		sendErrorCode(client, ERR_ERRONEUSNICKNAME, "");
		return ;
	}
	std::string message = client->getNickName() + " changed his nickname to " + buffer + ".\r\n";
	send(client->getFd(), message.c_str(), message.length(), 0);
	client->setNickName(buffer);
	client->setIsNick(true);
	if (client->getIsUser())
		client->setIsRegistered(true);
}

/**
 * @brief Handles the /USER command
 * @param client the pointer of the client
 * @param buffer the parameters of the command
 */
void Command::user(Client* client, std::string buffer)
{
	std::vector<std::string>	params;
	std::stringstream			ss(buffer);
	std::string					token;

	if (client->getIsUser())
	{
		sendErrorCode(client, ERR_ALREADYREGISTERED, "");
		return ;
	}

	while (ss >> token)
	{
		if (token[0] == ':')
		{
			std::string real = token.substr(1);
			std::string tmp;
			while (ss >> tmp)
				real += " " + tmp;
			params.push_back(real);
			break;
		}
		params.push_back(token);
	}
	if (params.size() < 4)
	{
		sendErrorCode(client, ERR_NEEDMOREPARAMS, "USER"); // "Syntax : /USER <username> 0 * :<realname>"
		return;
	}
	if (params.size() > 4)
	{
		sendErrorCode(client, ERR_NEEDMOREPARAMS, "USER"); // "You need \":\" before your realname if it contains spaces."
		return;
	}
	client->setIsRegistered(true);
	std::string message = client->getNickName() + " set his username to " + params[0] + " (" + params[3] + ").\r\n";
	client->setUserName(params[0]);
	client->setRealName(params[3]);
	client->setIsUser(true);
	if (client->getIsNick())
		client->setIsRegistered(true);
	send(client->getFd(), message.c_str(), message.length(), 0);
	// sendWelcome(client); TODO (see at bottom)
}

/**
 * @brief Create new channel if he exist
 * @param channelV channel name
 * @param keyV password
 * 
*/
void	Command::join(Client* client, std::string buffer)
{	
	std::vector<std::string> channelV;
	std::vector<std::string> keyV;
	std::string channelStr;
	std::string keyStr;

	std::stringstream ss(buffer);
	ss >> channelStr;
	if (!ss.eof())
		ss >> keyStr;
	std::stringstream channelSS(channelStr);
	std::string out;
	while (std::getline(channelSS, out, ','))
	{
		if (out[0] == '#' || out[0] == '&' || out[0] == '!' || out[0] == '+')
			channelV.push_back(out);
	}
	if (!keyStr.empty())
	{
		std::stringstream keySS(keyStr);
		std::string keyS;
		while (std::getline(keySS, keyS, ','))
			keyV.push_back(keyS);
	}
	
	for (size_t i = 0; i < channelV.size(); i++)
		std::cout << "channel: " << channelV[i] << std::endl;
	for (size_t i = 0; i < keyV.size(); i++)
		std::cout << "key: " << keyV[i] << std::endl;
	if (keyV.size() > channelV.size()) //TODO
		return; // sendError //TODO

	for (size_t i = 0; i < channelV.size(); i++)
	{
		std::string channelName = channelV[i];
		std::string pass;
		if (i < keyV.size())
			pass = keyV[i];
		else
			pass = "";
		Channel* channel = server->findChannel(channelName);
		if (channel == NULL) // Canal don't exist, we create it
		{
			channel = new Channel(channelName, client);
			server->addChannel(channelName, channel);
		}
		channel->addUser(pass, client);
	// 332, 353 et 366 rpl mandatory TODO
	// 400- 599 error code

	}
}

void Command::mode(Client* client, std::string buffer) //TODO
{
	if (buffer.empty())
	{
		sendErrorCode(client, ERR_NEEDMOREPARAMS, "MODE");
		return;
	}
	std::string msg = ":" + client->getHostname() +  " 324 " 
						+ client->getNickName() 
						+ " " + buffer + " +\r\n"; // RPL_CHANNELMODEIS (324)
	send(client->getFd(), msg.c_str(), msg.size(), 0);
}

void Command::who(Client* client, std::string buffer) //TODO
{
	if (buffer.empty())
	{
		sendErrorCode(client, ERR_NEEDMOREPARAMS, "MODE");
		return;
	}
	std::string msg = client->getFd() + " " + buffer 
						+ " " + client->getUser() 
						+ " " + client->getHostname() 
						+ client->getNickName() + " :" 
						+ "0 " + client->getRealName() + "\r\n"; //RPL_WHOREPLY (352)
	send(client->getFd(), msg.c_str(), msg.size(), 0);
}

void Command::privmsg(Client* client, std::string buffer)
{
	(void)*client;
	std::stringstream ss(buffer);
	std::string channelName;
	std::string message;
	ss >> channelName;
	std::getline(ss, message);
	message.erase(0, 2);

	Channel* channel = server->findChannel(channelName);
	std::string ircMsg = ":" + client->getNickName() + "!" 
									+ client->getUser() + "@" 
									+ client->getHostname() 
									+ " PRIVMSG " + channelName	 
									+ " :" + message + "\r\n";
	channel->sendAllChan(ircMsg);
}


void	Command::test(Client* client, std::string buffer)
{
	std::string response = "TEST reçu! Paramètres: '" + buffer + "'\r\n";
	send(client->getFd(), response.c_str(), response.length(), 0);
}

// std::string Command::codeToString(int value) // TODO, test again this format std::string (like lorenzo) for sendError(actualy stringstream)
// {
//     std::stringstream ss;
//     ss << value;
//     return ss.str();
// }

/*

Command:
	authentication:
		- PASS <password>
		- NICK <nickname>
		- USER <username> <hostname> <servername> <realname>
	canal:
		- JOIN <#canal>
		- PART <#canal>
	
	message: 
		- For canal<#> or an user: PRIVMSG <target> <message>
	operator:
		- KICK
		- INVITE
		- TOPIC
		- MODE 
			flags:  i, t, k, o, l
	
	!!WARNING!! need to handle \r\n in each command
	
#
*/

/// MESSAGE PROCESSING ///
// void Serv::processMessage(int user_fd, const char *message)
// {
// 	std::string msg(message);
// 	std::string buffer;

// 	this->clients[user_fd]->appendToBuffer(msg);
// 	buffer = this->clients[user_fd]->getBuffer();
// 	if (buffer.find_first_of("\r\n") == std::string::npos)
// 		return;

// 	std::vector<std::string> commands = splitCommands(buffer);
// 	for (std::vector<std::string>::iterator it = commands.begin(); it != commands.end(); ++it)
// 		this->interpret_message(user_fd, *it);
// 	if (this->clients[user_fd])
// 		this->clients[user_fd]->clearBuffer();
// }

/*sendWelcome(client) TODO

RPL_WELCOME (001)

RPL_YOURHOST (002)

RPL_CREATED (003)

RPL_MYINFO (004)*/