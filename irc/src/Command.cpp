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
	// CommandMap["NICK"] = &Command::nick;
	// CommandMap["USER"] = &Command::user;
	CommandMap["JOIN"] = &Command::join;
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



void	Command::join(Client* client, std::string buffer)
{
	(void)*client;
	
	std::vector<std::string> channelV;
	std::string channelStr;
	std::vector<std::string> keyV;
	std::string keyStr;

	std::stringstream ss(buffer);
	std::cout << "buffer: " << buffer << std::endl;
	ss >> channelStr;
	if (!ss.eof())
		ss >> keyStr;
	std::stringstream channelSS(channelStr);
	std::string out;
	while (std::getline(channelSS, out, ','))
	{
		if (!out.empty() && out[0] == '#')
			out.erase(0, 1);
		channelV.push_back(out);
	}
	

	if (!keyStr.empty())
	{
		std::stringstream keySS(keyStr);
		std::string keyS;
		while (std::getline(keySS, keyS, ','))
			keyV.push_back(keyS);
	}
	
	std::cout << "channel: " << std::endl;
	for (size_t i = 0; i < channelV.size(); i++)
		std::cout << "channel: " << channelV[i] << std::endl;
	std::cout << "key: " << std::endl;
	for (size_t i = 0; i < keyV.size(); i++)
		std::cout << "key: " << keyV[i] << std::endl;
	// if (keyV.size() > channelV.size()) TODO
	// 	send error
	// // TODO condition if to compare the channel name with the password with the mode +k


	// while (ss >> token)
	// {
	// 	// if (chanKey.size() > 2)
	// 	// 	return error // TODO
	// 	channel.push_back(token);
	// }
	// for (size_t i = 0; i < chanKey.size(); i++)
	// 	std::cout << "channel: " << chanKey[i] << std::endl;
	
	// if (chanKey[0].find('#', 0) != std::string::npos) //erase #
	// {
	// 	chanKey[0].erase(0, 1);
	// 	while (chanKey[0].find('#', 0) != std::string::npos)
	// 		chanKey[0].erase(chanKey[0].find('#', 0), 1);
	// 	std::cout << "channel: " << chanKey[0] << std::endl;

	// }
	// std::string channel = chanKey[0];
	// if ()
	


}


// void	pass_serv(Client* client, std::string buffer)
// {
// 	if (buffer == "" || buffer != )
// 		//TODO deconnection;
	
// }

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

// TEMP
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
