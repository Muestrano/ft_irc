#include "../include/Channel.hpp"
#include "../include/include.hpp"

Channel::Channel(std::string name, Client* clientOp) : name(name), password(""), topic(""), limitMember(0), nbMember(0), nbOperator(0), invitedOnly(false)
{
	members[clientOp->getNickName()] = clientOp;
	nbMember = 1;
	operators[clientOp->getNickName()] = clientOp;
}

Channel::~Channel()
{
}

// Getters 

std::map<std::string, Client*>& Channel::getMembers() 
{ 
	return members;
}

std::string Channel::getTopic() const 
{
	return topic;
}

std::string Channel::getName() const
{
	return name;
}

// Public method

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
	{
		nbOperator++;
		operators[nickName] = client;
	}
	std::string message = ":" + client->getNickName() + "!" + client->getUser() + "@" + client->getHostname() + " JOIN " + this->name + "\r\n";
	sendAllChan(message);
	std::cout << "new client: " << nickName << std::endl;
}

/**
 * @brief msg to all user of one channel when someone joint it
 * @param MSG_CONFIRM flag confirm the msg is send
*/
void Channel::sendAllChan(std::string message)
{
	std::cout << message << std::endl;
	std::map<std::string, Client*>::iterator it;
	it = members.begin();
	while(it != members.end())
	{
		send(it->second->getFd(), message.c_str(), message.size(), 0); //MSG_CONFIRM TEMP (check this flag)
		it++;
	} 
}

/**
 * @brief Send message to all channel members except one client (used for the sender, NULL for no exception none)
 * // TODO Replace sendAllChan call by this one.
 */
void Channel::sendAllChanExcept(std::string message, Client* exclude)
{
	std::cout << message << std::endl;
	std::map<std::string, Client*>::iterator it;
	
	it = members.begin();
	while(it != members.end())
	{
		if (exclude == NULL || it->second->getFd() != exclude->getFd())
			send(it->second->getFd(), message.c_str(), message.size(), 0);
		it++;
	}
}

/**
 * @brief Check if a client is member of this channel.
 *
 */
bool Channel::isMember(Client* client)
{
    std::string nickName = client->getNickName();
    std::map<std::string, Client*>::iterator it = members.find(nickName);
    return (it != members.end());
}

bool Channel::isOperator(Client* client)
{
	std::string nickName = client->getNickName();
    std::map<std::string, Client*>::iterator it = operators.find(nickName);
	return (it != operators.end());
}

void Channel::removeMember(Client* client)
{
	std::map<std::string, Client*>::iterator it;
	it = members.find(client->getNickName());
	if (it != members.end())
	{
		this->nbMember--;
		this->members.erase(it);
	}
	std::map<std::string, Client*>::iterator itOp;
	itOp = operators.find(client->getNickName());
	// if (itOp != operators.end()) //TODO Check if we need nbOperator
	// {
	// 	nbOperator--;
	// }
}

bool Channel::chanIsEmpty()
{
	if (nbMember == 0)
		return (true);
	return (false);
}
