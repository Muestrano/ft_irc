#include "../include/Client.hpp"
#include "../include/include.hpp"

/*
	handle client connection / received data (non blocking)
	For handle non blocking we can put data into buffer
*/

Client::Client()
{

}
Client::Client(int fd) : clientFd(fd)
{

}
Client::~Client()
{

}
void Client::setNickName(const std::string &nick)
{
	this->nickName = nick;
}
void Client::setUser(const std::string &user)
{
	this->userName = user;
}
void Client::setHostname(const std::string& hostname)
{
	this->hostName = hostname;
}

std::string Client::getNickName()
{
	return (this->hostName);
}
std::string Client::getUser()
{
	return (this->userName);
}
std::string Client::getHostname()
{
	return (this->hostName);
}
