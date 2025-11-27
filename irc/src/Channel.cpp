#include "../include/Channel.hpp"
#include "../include/include.hpp"

Channel::Channel(std::string name, Client* clientOp) : name(name), password(""), topic(""), limitMember(0), nbMember(0), invitedOnly(false)
{
	members[clientOp->getNickName()] = clientOp;
	nbMember = 1;
	operators[clientOp->getNickName()] = clientOp;
}
/**
 * ´
*/
void	Channel::addUser(const std::string key, Client* client)
{
	std::string nickName = client->getNickName();
	// need to Verif modes //TODO
	if (!password.empty() && key != password )
	{
		// sendError(ERR_BADCHANNELKEY) //TODO
		return;
	}
	if (limitMember > 0 && nbMember >= limitMember)
	{
		// sendError(ERR_CHANNELISFULL) //TODO
		return;
	}
	if (invitedOnly /*&& isInvited(client) TODO*/)
	{
		// sendError(ERR_INVITEONLYCHAN) //TODO
		return;
	}
	if (members.find(nickName) == members.end())
	{
		members[nickName] = client;
		nbMember++;
	}
	else // client is already in the channel
		// sendError() //TODO
	
	if (nbMember == 1)
		operators[nickName] = client;
	// sendJoinMsg(client); //TODO
	std::cout << "new client: " << nickName << std::endl;
}
