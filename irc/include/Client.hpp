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
		Client(int fd);
		// ~Client();
		std::string getNickName() const;
		std::string getUser() const;
		std::string getHostname() const;
		std::string getBuffer() const;
		int getFd() const;


		void setNickName(const std::string& nick);
		void setUser(const std::string& user);
		void setHostname(const std::string& hostname);
		void setBuffer(const std::string& buffer);


};

#endif