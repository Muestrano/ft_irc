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
	CommandMap["TEST"] = &Command::ft_test;
	// CommandMap["PASS"] = &Command::ft_pass_chan;
	// CommandMap["NICK"] = &Command::ft_nick;
	// CommandMap["USER"] = &Command::ft_user;
	// CommandMap["JOIN"] = &Command::ft_join;
	// CommandMap["MODE"] = &Command::ft_mode;
	// CommandMap["TOPIC"] = &Command::ft_topic;
	// CommandMap["INVITE"] = &Command::ft_invite;
	// CommandMap["KICK"] = &Command::ft_kick;
	// CommandMap["PRIVMSG"] = &Command::ft_privmsg;
	// CommandMap["PART"] = &Command::ft_exit;
	// CommandMap["QUIT"] = &Command::ft_quit;

}
void	Command::ft_test(Client* client, Server* server, std::string buffer)
{
	(void)server;
    std::string response = "TEST reçu! Paramètres: '" + buffer + "'\r\n";
    send(client->getFd(), response.c_str(), response.length(), 0);
}

// void ft_join(std::string buffer, int client)
// {
// }

// }
/**
 * @brief Extract all command to pass it in the handler command
 * @param buffer Input from the client
 * @param client Client's fd 
*/
void Command::extractCompleteCommand(Client* client, Server* server)
{
	std::string& buffer = client->getBuffer();
	
	size_t pos;
	while ((pos = buffer.find("\r\n")) != std::string::npos) 
	{
		std::string line = buffer.substr(0, pos);
		buffer.erase(0, pos + 2); // Supp \r\n for the next  command        
		prepareCommand(client, server, line);
	}
	buffer.erase(0, pos + 2);
}

// std::string Command::codeToString(int value) // TODO, test again this format std::string (like lorenzo) for sendError(actualy stringstream)
// {
//     std::stringstream ss;
//     ss << value;
//     return ss.str();
// }

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

void	Command::ft_test(Client* client, Server* server, std::string buffer)
{
	(void)server;
    std::string response = "TEST reçu! Paramètres: '" + buffer + "'\r\n";
    send(client->getFd(), response.c_str(), response.length(), 0);
}

/**
 * @brief extract the command and params to handle it
*/
// this function don't handle the different name of command
void Command::prepareCommand(Client* client, Server* server, std::string line)
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
		(this->*funcMap)(client, server, param); 
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
