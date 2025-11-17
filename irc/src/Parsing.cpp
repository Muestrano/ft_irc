#include "../include/Server.hpp"
#include "../include/include.hpp"

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