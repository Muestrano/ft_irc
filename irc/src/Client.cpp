#include "../include/Client.hpp"
#include "../include/include.hpp"

/*
	handle client connection / received data (non blocking)
	For handle non blocking we can put data into buffer
*/

Client::Client(int fd) : clientFd(fd)
{

}
// Client::~Client()
// {
// 	delete Client;
// }


// setter
void Client::setBuffer(const std::string& Buffer)
{
	this->buffer = Buffer;
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
// getter
std::string Client::getNickName() const
{
	return (this->hostName);
}
std::string Client::getUser() const
{
	return (this->userName);
}
std::string Client::getHostname() const
{
	return (this->hostName);
}

std::string Client::getBuffer() const
{
	return (this->buffer);
}
int Client::getFd() const
{
	return (this->clientFd);
}
