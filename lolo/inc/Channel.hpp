#pragma once

#include <string>
#include <iostream>
#include <map>
#include "Client.hpp"
#include "irc.hpp"

class Client;

class Channel
{
	private:
		std::string _name;
		std::string _password;
		std::string _topic;
		bool _inviteOnly;
		bool _restrictTopic;
		bool _limitUser;
		bool _passwordUse;
		int _nUser;
		int	_limit;
		std::map<std::string, Client*> _operators;
		std::map<std::string, Client*> _regulars;
		std::map<std::string, Client*> _invited;

	public:

		/* Constructor */
		Channel(std::string name, Client *creator);
		Channel(std::string name, std::string password, Client *creator);
		~Channel();

		/* Getter */
		const std::string &getName() const;
		int clientInChannel(Client *user);
		int getNBUser(void);
		/* Sender */
		void sendNumericResponse(Client* client, const std::string& code, const std::string& param1, const std::string& param2);
		void SendAllFD(const std::string& message, int fd);
		void sendAll(const std::string& message);
		static void sendMessage(Client* client, const std::string& msg);
		void sendAllUser(Client *user);
		/* Channel */
		static void createChannel(const std::string& name, Client* creator);
		void kick(Client* creator, const std::string& targetNickname);
		void addUser(Client* user, std::string password);
		void removeUser(Client* user);
		void invite(Client* sender, Client* newUser);
		void topic(Client* sender, const std::string& newTopic);
		void quitChannel(Client* client, std::string mess);
		/* Mode */
		void checkMode(std::string *mess);
		void modifMode(char modeSign, char modeChar, std::string *param);
		void setPassword(std::string param);
};
