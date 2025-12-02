#include "../include/Channel.hpp"
#include "../include/include.hpp"

Channel::Channel(std::string name, Client* clientOp) : name(name), password(""), topic(""), limitMember(0), nbMember(0), invitedOnly(false)
{
	members[clientOp->getNickName()] = clientOp;
	nbMember = 1;
	operators[clientOp->getNickName()] = clientOp;
}

/**
 * @brief msg to all user of one channel when someone joint it
 * @param MSG_CONFIRM flag confirm the msg il send
*/	
void Channel::sendAllChan(std::string message)
{
	std::cout << "HHHHHHHHHHHH" << std::endl;
	std::map<std::string, Client*>::iterator it;
	it = members.begin();
	while(it != members.end())
	{
		send(it->second->getFd(), message.c_str(), message.size(), 0); //MSG_CONFIRM TEMP (check this flag)
		it++;
	} 
}

/**
 * @brief check all Mode operator before add user in std::map members
*/
void	Channel::addUser(const std::string key, Client* client)
{
	Command command;
	std::string nickName = client->getNickName();
	// need to Verif modes //TODO
	if (!password.empty() && key != password )
	{
		command.sendErrorCode(client, ERR_BADCHANNELKEY, this->name); //TODO
		return;
	}
	if (limitMember > 0 && nbMember >= limitMember)
	{
		command.sendErrorCode(client, ERR_CHANNELISFULL, this->name); //TODO
		return;
	}
	if (invitedOnly /*&& isInvited(client) TODO*/)
	{
		command.sendErrorCode(client, ERR_INVITEONLYCHAN, this->name); //TODO
		return;
	}
	if (members.find(nickName) == members.end())
	{
		members[nickName] = client;
		nbMember++;
	}
	if (nbMember == 1)
		operators[nickName] = client;
	std::string message = ":" + client->getNickName() + "!" + client->getUser() + "@" + client->getHostname() + " JOIN " + this->name + "\r\n";
	sendAllChan(message);
	std::cout << "new client: " << nickName << std::endl;
	// std::map<std::string, Client*>::iterator it;
	// it = members.begin();
	// while (it != members.end())
	// {
	// 	std::cout << "client member: " << it->first << std::endl;
	// 	it++;
	// }
}
