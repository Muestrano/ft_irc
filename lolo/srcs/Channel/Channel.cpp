#include "../../inc/Channel.hpp"

Channel::Channel(const std::string name, Client *creator) : _name(name), _password(""), _topic(""), _inviteOnly(false),
			_restrictTopic(true), _limitUser(false), _passwordUse(false), _nUser(1), _limit(0)
{
	_regulars.insert(std::pair<std::string, Client*>(creator->getNickname() , creator));
	_operators.insert(std::pair<std::string, Client*>(creator->getNickname() , creator));

	sendMessage(creator, ":127.0.0.1 332 " + creator->getNickname() + " #" + _name + " :Welcome to the " + _name +  " " + "Channel!\n");

	std::string joinMsg = ":" + creator->getNickname() + "!~" + creator->getUsername()[0] + "@127.0.0.1 JOIN #" + name + "\n";
	sendMessage(creator, joinMsg);

	std::string modeMsg = ":127.0.0.1 MODE #" + name + " +nt\n"; // RPL_CHANNELMODEIS
	sendMessage(creator, modeMsg);

	std::string namesMsg = ":127.0.0.1 353 " + creator->getNickname() + " = #" + _name + " :@" + creator->getNickname() + "\n"; // RPL_NAMREPLY
	sendMessage(creator, namesMsg);

	std::string endNamesMsg = ":127.0.0.1 366 " + creator->getNickname() + " #" + name + " :End of /NAMES list.\n"; // RPL_ENDOFNAMES
	sendMessage(creator, endNamesMsg);
}

Channel::Channel(std::string name, std::string password, Client *creator) : _name(name), _password(password), _topic(""), _inviteOnly(false),
			_restrictTopic(true), _limitUser(false), _passwordUse(true), _nUser(1), _limit(0)
{
	_operators[creator->getNickname()] = creator;
	_regulars[creator->getNickname()] = creator;

	sendMessage(creator, ":127.0.0.1 332 " + creator->getNickname() + " #" + _name + " :Welcome to the" + _name + "Channel!\n");

	std::string joinMsg = ":" + creator->getNickname() + "!~" + creator->getUsername()[0] + "@127.0.0.1 JOIN #" + name + "\n";
	sendMessage(creator, joinMsg);

	std::string modeMsg = ":127.0.0.1 MODE #" + name + " +nt\n"; // RPL_CHANNELMODEIS
	sendMessage(creator, modeMsg);

	std::string namesMsg = ":127.0.0.1 353 " + creator->getNickname() + " = #" + _name + " :@" + creator->getNickname() + "\n"; // RPL_NAMREPLY
	sendMessage(creator, namesMsg);

	std::string endNamesMsg = ":127.0.0.1 366 " + creator->getNickname() + " #" + name + " :End of /NAMES list.\n"; // RPL_ENDOFNAMES
	sendMessage(creator, endNamesMsg);
}

Channel::~Channel()
{

}

void Channel::sendMessage(Client* client, const std::string& msg)
{
	send(client->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM); // MSG_CONFIRM is a flag to tell the receiver that the data was received correctly
}

const std::string& Channel::getName() const
{
	return (_name);
}

int	Channel::getNBUser(void)
{
	return (_nUser);
}

void Channel::sendNumericResponse(Client* client, const std::string& code, const std::string& param1, const std::string& param2)
{
	std::string message = ":127.0.0.1 " + code + " " + param1 + " ";

	if (!param2.empty())
		message += param2;
	message += "\n";
	sendMessage(client, message);
}

void Channel::sendAll(const std::string& message)
{
	for (std::map<std::string, Client*>::const_iterator it = _regulars.begin(); it != _regulars.end(); ++it)
		sendMessage(it->second, message);
}
void Channel::kick(Client* creator, const std::string& targetNickname)
{
	std::map<std::string, Client*>::iterator targetRegular = _regulars.find(targetNickname);
	std::map<std::string, Client*>::iterator targetOperator = _operators.find(targetNickname);

	if (targetRegular == _regulars.end())
		sendNumericResponse(creator, "442", creator->getNickname(), _name);
	else if (targetOperator != _operators.end())
		sendMessage(creator, ":127.0.0.1 Error :You can't kick an operator\n");
	else if (_operators.find(creator->getNickname()) == _operators.end())//verif target est dans le chan
		sendNumericResponse(creator, "482", creator->getNickname(), "ERR_CHANOPRIVSNEEDED"); // you doesn't have the operator right
	else
	{
		std::string kickMessage = ":" + creator->getNickname() + "!~" + creator->getUsername() + "@127.0.0.1 KICK #" + _name + ' ' + targetRegular->second->getNickname() + " :\n";
		sendAll(kickMessage);
		_regulars.erase(targetRegular);
	}
}

void Channel::addUser(Client* user, std::string password)
{
	std::string	msg;
	if (password != _password && _passwordUse == true)
	{
		sendNumericResponse(user, "475", user->getNickname(), "ERR_BADCHANNELKEY");
		return ;
	}
	else if (_limitUser == true && _limit <= _nUser + 1)
	{
		sendNumericResponse(user, "471", user->getNickname(), "ERR_CHANNELISFULL");
		return ;
	}
	else if (_inviteOnly == false)
	{
		_regulars.insert(std::pair<std::string, Client*>(user->getNickname() , user));
		_nUser++;
		sendAllUser(user);
		if (_topic == "")
			msg = ":127.0.0.1 331 " + user->getNickname() + " #" + _name + " :No topic set\n"; // RPL_NOTOPIC
		else
			msg = ":127.0.0.1 332 " + user->getNickname() + " #" + _name + " :" + _topic + "\n"; // RPL_TOPIC
		sendMessage(user, msg);
		msg = ":127.0.0.1 353 " + user->getNickname() + " = #" + _name + " :"; // RPL_NAMREPLY
		for (std::map<std::string, Client *>::iterator it = _regulars.begin(); it != _regulars.end(); it++) {
			if (_operators.find(it->second->getNickname()) != _operators.end()) {
				msg += "@";
			}
			msg += it->second->getNickname() + " ";
		}
		msg += "\n";
		sendMessage(user, msg);
		msg = ":127.0.0.1 366 " + user->getNickname() + " #" + _name + " :End of NAMES list\n"; // RPL_ENDOFNAMES
		sendMessage(user, msg);
		_regulars[user->getNickname()] = user;
	}
	else
	{
		sendNumericResponse(user, "473", user->getNickname(), "ERR_INVITEONLYCHAN");
		return ;
	}
}

void	Channel::sendAllUser(Client *user)
{
	std::string msg = ":" + user->getNickname() + "!~" + user->getUsername()[0] + "@127.0.0.1 JOIN :#" + _name + "\n";
	for (std::map<std::string, Client *>::iterator it = _regulars.begin(); it != _regulars.end(); it++)
		send(it->second->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
}

void Channel::removeUser(Client* user)
{
	_regulars.erase(user->getNickname());
	_operators.erase(user->getNickname());
	_nUser--;
}

void Channel::invite(Client* sender, Client* newUser) {
    // Check if the sender is an operator of the channel
    if (_operators.find(sender->getNickname()) == _operators.end()) {
        if (_inviteOnly) {
            sendNumericResponse(sender, "482", sender->getNickname(), _name); // ERR_CHANOPRIVSNEEDED
            return;
        }
    }

	// Check if the target user is already in the channel
	if (_regulars.find(newUser->getNickname()) != _regulars.end()) {
		sendNumericResponse(sender, "443", newUser->getNickname(), _name);
		return;
	}

    // Send a message to the target user asking them to join the channel
    if (newUser != NULL) {
		std::string inviteMessage = ":" + sender->getNickname() + "!~" + sender->getUsername() + "@127.0.0.1" + " INVITE " + newUser->getNickname() + " :" + _name + "\n";
		sendMessage(newUser, inviteMessage);
    } else {
        sendNumericResponse(sender, "401", sender->getNickname(), ""); // ERR_NOSUCHNICK
    }

	// if channel has a password, allow the user to join the channel
	if (_passwordUse) {
		_regulars.insert(std::pair<std::string, Client*>(newUser->getNickname(), newUser));
		_nUser++;
		sendAllUser(newUser);
	}
}

void Channel::topic(Client* sender, const std::string& newTopic) {
    // Check if the sender is an operator of the channel or the only person in it
    if (_restrictTopic && _operators.find(sender->getNickname()) == _operators.end()) {
        sendNumericResponse(sender, "482", sender->getNickname(), _name); // ERR_CHANOPRIVSNEEDED
        return;
    }

	// If command is just topic, show the current topic
	if (newTopic.empty())
	{
		if (_topic == "")
			sendNumericResponse(sender, "331", sender->getNickname(), _name); // RPL_NOTOPIC
		return;
	}

    // If everything is fine, change the topic and notify all users about this event
    _topic = newTopic;
    std::string topicMessage = ":" + sender->getNickname() + "!~" + sender->getUsername() + "@127.0.0.1 TOPIC #" + _name + " :" + _topic + "\n";
    sendAll(topicMessage);

    // Notify the sender about the successful topic change
    sendNumericResponse(sender, "332", sender->getNickname(), _name); // RPL_TOPIC
}

void Channel::quitChannel(Client* client, std::string mess)
{
	std::map<std::string, Client*>::iterator it = _regulars.find(client->getNickname());
	if (it != _regulars.end())
	{
		_regulars.erase(it);
		_nUser--;
	}
	it = _operators.find(client->getNickname());
	if (it != _operators.end())
		_operators.erase(it);
	std::string msg = ":" + client->getNickname() + "!~" + client->getUsername() + "@127.0.0.1" + " PART #" + _name + " :" + mess + "\n";
	sendAll(msg);
}

void Channel::checkMode(std::string *mess)
{
	std::string *paramString;

	char modeSign = mess[0][0];
	char modeChar = mess[0][1];
	if (mess[1][0])
		paramString = mess + 1;
	else
		paramString = 0;
	modifMode(modeSign, modeChar, paramString);
}

void Channel::modifMode(char modeSign, char modeChar, std::string *param)
{
	if (modeSign == '+')
	{
		if (modeChar == 'i')
		{
			if (_inviteOnly == false)
				_inviteOnly = true;
		}
		else if (modeChar == 't')
		{
			if (_restrictTopic == false)
				_restrictTopic = true;
		}
		else if (modeChar == 'k')
		{
			if (_passwordUse == false)
			{
				if (param)
					setPassword(*param);
			}
		}
		else if (modeChar == 'o')
        {
			size_t i = 0;
			while (!param[i].empty())
			{
    			if (_operators.find(param[i]) != _operators.end())
        			return ;
    			std::map<std::string, Client*>::iterator it = _regulars.find(param[i]);
				if (it != _regulars.end())
    			{
        			Client* user = it->second;
        			_operators[param[i]] = user;
    			}
    			++i;
			}
        }
        else if (modeChar == 'l')
        {
			if (param)
			{
				_limitUser = true;
				_limit = std::atoi(param[0].c_str());
			}
		}
	}
	else if (modeSign == '-')
	{
		if (modeChar == 'i')
		{
			if (_inviteOnly == true)
				_inviteOnly = false;
		}
		else if (modeChar == 't')
		{
			if (_restrictTopic == true)
				_restrictTopic = false;
		}
		else if (modeChar == 'k')
		{
			if (_passwordUse == true)
			{
				_passwordUse = false;
				_password = "";
			}
		}
		else if (modeChar == 'o')
        {
			size_t i = 0;
			while (!param[i].empty())
			{
    			if (_operators.find(param[i]) == _operators.end())
					return ;
				std::map<std::string, Client*>::iterator it = _operators.find(param[i]);
				if (it != _operators.end())
					_operators.erase(it);
				i++;
			}
        }
        else if (modeChar == 'l')
        {
            if (_limitUser == true)
            {
                _limitUser = false;
                _limit = 0;
            }
        }
	}
}

void Channel::setPassword(std::string param)
{
	_passwordUse = true;
	_password = param;
}

void Channel::SendAllFD(const std::string& message, int fd)
{
	for (std::map<std::string, Client*>::const_iterator it = _regulars.begin(); it != _regulars.end(); ++it)
	{
		if (it->second->getFd() != fd)
			sendMessage(it->second, message);
	}
}

int Channel::clientInChannel(Client *user)
{
	std::map<std::string, Client*>::iterator it = _operators.find(user->getNickname());
	if (it != _operators.end())
		return (2);
	it = _regulars.find(user->getNickname());
	if (it != _regulars.end())
		return (1);
	return (0);
}
