#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "include.hpp"

/*
	connected user (his socket, nick, username...)
	
*/
class Client
{
	private:
		std::string nickName;
		std::string userName;

		int clientFd;

	public:
		Client();
		Client(int fd);
		~Client();

		void setNickName(const std::string &nick);
		void setUser(const std::string &user);
};

#endif