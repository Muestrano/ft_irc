#include "../include/Server.hpp"
#include "../include/include.hpp"

Command::Command()
{
	set_map();
}

Command::Command(const Command& c)
{
	*this = c;
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
	CommandMap["PASS"] = &Command::ft_pass_chan;
	CommandMap["NICK"] = &Command::ft_nick;
	CommandMap["USER"] = &Command::ft_user;
	CommandMap["JOIN"] = &Command::ft_join;
	CommandMap["MODE"] = &Command::ft_mode;
	CommandMap["TOPIC"] = &Command::ft_topic;
	CommandMap["INVITE"] = &Command::ft_invite;
	CommandMap["KICK"] = &Command::ft_kick;
	CommandMap["PRIVMSG"] = &Command::ft_privmsg;
	CommandMap["PART"] = &Command::ft_exit;
	CommandMap["QUIT"] = &Command::ft_quit;

}
// /!\ A mettre dans les constructeurs

/**
 * @brief 
 * @param buffer Input from the client
 * @param client Client's fd
*/
void Command::ft_redirect_buffer(std::string buffer, int client)
{

}

void ft_join(std::string buffer, int client)
{

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


/*

/// MESSAGE PROCESSING ///
void Serv::processMessage(int user_fd, const char *message)
{
	std::string msg(message);
	std::string buffer;

	this->clients[user_fd]->appendToBuffer(msg);
	buffer = this->clients[user_fd]->getBuffer();
	if (buffer.find_first_of("\r\n") == std::string::npos)
		return;

	std::vector<std::string> commands = splitCommands(buffer);
	for (std::vector<std::string>::iterator it = commands.begin(); it != commands.end(); ++it)
		this->interpret_message(user_fd, *it);
	if (this->clients[user_fd])
		this->clients[user_fd]->clearBuffer();
}

*/