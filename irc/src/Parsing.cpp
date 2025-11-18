#include "../include/Server.hpp"
#include "../include/include.hpp"

/*

	First: pars port, ref the to port of an irc server
			Parse ipv4 with the great range

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