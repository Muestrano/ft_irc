#include "../../inc/Client.hpp"

Client::Client()
{

}

Client::Client(int fd) : _fdClient(fd)
{

}

Client::~Client()
{

}

std::string const& Client::getNickname() const
{
    return (_nickname);
}

std::string const& Client::getUsername() const
{
    return (_username);
}

int Client::getFd() const
{
    return (_fdClient);
}

void Client::setNickname(const std::string &nickname)
{
    _nickname = nickname;
}

void Client::setUsername(const std::string &username)
{
    _username = username;
}

