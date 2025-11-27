#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "include.hpp"

class Client
{
	private:

		std::string nickName;
		std::string userName;
		std::string realName; 
		std::string hostName; // IP client
		std::string passWord;
		std::string buffer;
		
		bool isAuthenticated;

		bool	Nick;
		bool	User;
		bool	isRegistered; // Pass -> Nick -> User

		int 		clientFd;

	public:
		Client();
		Client(int fd);
		~Client();

		// getters
		std::string getNickName() const;
		std::string getUser() const;
		std::string getHostname() const;
		std::string& getBuffer();
		int getFd() const;
		bool getIsRegistered() const;
		bool getIsAuthenticated() const;

		// setters
		void setRealName(const std::string& real);
		void setHostname(const std::string& hostname);
		void setNickName(const std::string& nick);
		void setUser(const std::string& user);
		void setBuffer(const std::string& buffer);
		void setIsRegistered(const bool registered);
		void setIsAuthenticated(bool isAuthenticated);
};

#endif