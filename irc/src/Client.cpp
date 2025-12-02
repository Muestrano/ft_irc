#include "../include/Client.hpp"
#include "../include/include.hpp"

/*
	handle client connection / received data (non blocking)
	For handle non blocking we can put data into buffer
*/

Client::Client() : nickName(""), userName(""), hostName("127.0.0.1"), isAuthenticated(false), isRegistered(false)
{
}

Client::Client(int fd) : nickName(""), userName(""), hostName("127.0.0.1"), isAuthenticated(false), isRegistered(false), clientFd(fd)
{
}

Client::~Client()
{
	// TODO : Define if the delete is usefull 
	//delete Client;
}

// Getters

std::string Client::getNickName() const
{
	return (this->nickName);
}

std::string Client::getUser() const
{
	return (this->userName);
}

std::string Client::getHostname() const
{
	return (this->hostName);
}

std::string& Client::getBuffer()
{
	return (this->buffer);
}
std::string Client::getRealName() const
{
	return (this->realName);
}


int Client::getFd() const
{
	return (this->clientFd);
}

bool Client::getIsRegistered() const
{
	return (this->isRegistered);
}

bool Client::getIsAuthenticated() const
{
	return (this->isAuthenticated);
}

// Setters

void Client::setRealName(const std::string &real)
{
	this->realName = real;
}

void Client::setHostname(const std::string& hostname)
{
	this->hostName = hostname;
}

void Client::setNickName(const std::string &nick)
{
	this->nickName = nick;
}

void Client::setUser(const std::string &user)
{
	this->userName = user;
}

void Client::setBuffer(const std::string& appendBuffer)
{
	this->buffer += appendBuffer;
}

void Client::setIsRegistered(const bool registered)
{
	this->isRegistered = registered;
}

void Client::setIsAuthenticated(bool authenticated)
{
	this->isAuthenticated = authenticated;
}
