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
		std::string hostName;
		std::string buffer;

		int clientFd;

	public:
		Client();
		Client(int fd);
		~Client();
		
		std::string getNickName();
		std::string getUser();
		std::string getHostname();

		void setNickName(const std::string& nick);
		void setUser(const std::string& user);
		void setHostname(const std::string& hostname);

};

#endif