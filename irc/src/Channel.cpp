#include "../include/Channel.hpp"
#include "../include/include.hpp"

Channel::Channel(std::string name, Client* operator) : name(name), limitMember(0), nbMember(0), topic(""), password("")
{
	member[operator->nickname];
}

void	addUser(const std::string name, Client* client)
{
	Channel* newChannel = Channel(name, client);
	newChannel->member[client->getNickName]

}
