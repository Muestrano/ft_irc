#include "../include/Server.hpp"
#include "../include/include.hpp"

Command::Command()
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

/**
 * @brief Sets the CommandMap attribute
*/
void Command::set_map(void)
{
	CommandMap["TEST"] = &Command::test;
	// CommandMap["PASS"] = &Command::pass_serv;
	CommandMap["NICK"] = &Command::nick;
	CommandMap["USER"] = &Command::user;
	// CommandMap["JOIN"] = &Command::join;
	// CommandMap["MODE"] = &Command::mode;
	// CommandMap["TOPIC"] = &Command::topic;
	// CommandMap["INVITE"] = &Command::invite;
	// CommandMap["KICK"] = &Command::kick;
	// CommandMap["PRIVMSG"] = &Command::privmsg;
	// CommandMap["PART"] = &Command::exit;
	// CommandMap["QUIT"] = &Command::quit;

}
void	Command::test(Client* client, std::string buffer)
{
    std::string response = "TEST reçu! Paramètres: '" + buffer + "'\r\n";
    send(client->getFd(), response.c_str(), response.length(), 0);
}

void Command::sendError(Client* client, int codeError, const std::string& command)
{
	std::string nickname = client->getNickName();
    if (nickname.empty())
        nickname = "*";
    
    std::stringstream error;
    error << ":localhost " << codeError << " " << nickname;
    error << " " << command << " :Unknown command\r\n";
    
	std::string stringError = error.str();
    send(client->getFd(), stringError.c_str(), stringError.length(), 0);
}
void Command::sendErrorCode(Client* client, ErrorCode errorCode, std::string errorMsg)
{
	std::string nickname = client->getNickName();
    if (nickname.empty())
        nickname = "*";
    std::stringstream error;
    error << ":localhost " << "Error " << errorCode << " : ";
	switch (errorCode)
	{
	// case 421:
	// 	error << "Unknown command.";
	case 431:
		error << "Nickname is empty.";
		break;
	case 432:
		error << "The first character of your nickname cannot be \"#\" neither \":\". You can't have space character in your nickname. ";
		break;
	case 433:
		error << "This nickname is already registered. ";
		break;
	case 436:
		error << "Leo has been pre-shot. Try again bro! ";
		break;
	case 437:
		error << "Congrats Esteban! All test passed! 🫡 ";
		break;
	case 461:
		error << "This command needs more parameters. ";
		break;
	case 462:
		error << "You already registered your user name. ";
		break;
	default:
		break;
	}
	error << errorMsg << std::endl;
	std::string stringError = error.str();
    send(client->getFd(), stringError.c_str(), stringError.length(), 0);
}

void Command::nick(Client* client, std::string buffer)
{
	std::string	error;
	if (buffer.empty())
	{
		sendErrorCode(client, ERR_NONICKNAMEGIVEN, "");
		return ;
	}
	else if (buffer == client->getNickName())
	{
		sendErrorCode(client, ERR_NICKNAMEINUSE, "");
		return ;
	}
	else if (buffer[0] == '#' || buffer[0] == ':' || buffer.find(" ") != std::string::npos)
	{
		sendErrorCode(client, ERR_ERRONEUSNICKNAME, "");
		return ;
	}
	else if (buffer.find("zboub") != std::string::npos
			|| buffer.find("zgeg") != std::string::npos
				|| buffer.find("goumer")!= std::string::npos)
	{
		sendErrorCode(client, ERR_TARGETLEO, "");
		return ;
	}
	else if (buffer == "test" || buffer == "Test" || buffer == "TEST")
	{
		sendErrorCode(client, ERR_TARGETESTEBAN, "");
		return ;
	}
	std::string message = client->getNickName() + " changed his nickname to " + buffer + ".\r\n";
	send(client->getFd(), message.c_str(), message.length(), 0);
	client->setNickName(buffer);
}

/*sendWelcome(client) TODO

RPL_WELCOME (001)

RPL_YOURHOST (002)

RPL_CREATED (003)

RPL_MYINFO (004)*/

void Command::user(Client* client, std::string buffer)
{
	std::vector<std::string>	params;
	std::stringstream			ss(buffer);
	std::string					token;
	std::string					error;

	if (client->getIsRegistered())
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
		sendErrorCode(client, ERR_NEEDMOREPARAMS, "Syntax : /USER <username> 0 * :<realname>");
		return;
	}
	if (params.size() > 4)
	{
		sendErrorCode(client, ERR_NEEDMOREPARAMS, "You need \":\" before your realname if it contains spaces.");
		return;
	}
	client->setIsRegistered(true);
	std::string message = client->getNickName() + " set his username to " + params[0] + " (" + params[3] + ").\r\n";
	client->setUser(params[0]);
	client->setRealName(params[3]);
	send(client->getFd(), message.c_str(), message.length(), 0);
	// sendWelcome(client); TODO
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

// std::string Command::codeToString(int value) // TODO, test again this format std::string (like lorenzo) for sendError(actualy stringstream)
// {
//     std::stringstream ss;
//     ss << value;
//     return ss.str();
// }

/**
 * @brief extract the command and params to handle it
*/
// this function don't handle the different name of command
void Command::prepareCommand(Client* client, std::string line)
{
	std::istringstream ss(line);
	std::string command;
	ss >> command;

	std::cout << "line received: '" << line << "'" << std::endl;
	std::transform(command.begin(), command.end(), command.begin(), ::toupper); // upper conversion command

	std::string param;
	std::getline(ss, param);

	param.erase(0, param.find_first_not_of(" ")); //erase space befor param

	if (CommandMap.find(command) != CommandMap.end())
	{
		FtCommand funcMap = CommandMap[command]; // std::map<std::string, FtCommand> CommandMap;
		(this->*funcMap)(client, param); 
	}
	else
	{
		sendError(client, 421, command); // to do
	}
}

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
