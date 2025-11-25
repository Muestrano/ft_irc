#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "include.hpp"

class Channel
{
	private:
		// unsigned int 	limitMember;
		// unsigned int 	nbMember;
		// std::string 	topic;
		// std::string 	password;
		// bool			isOperator;

		// std::vector<Client*> members; //brodcast msg
		// std::map<std::string, Client*> member;
		// std::map<std::string, Client*> operators;
		// std::map<std::string, Client*> invited; // invite but not in the canal
		// What kind of container for list operator and member
	public:
	
};

/*

	Join: 
		- nbMember++
		-channel created automaticaly when the first member join it an 
			he is automatically operator 
		- Notif to the other member of the channel of there is new member
		- First newMember have the welcom msg
		- other newMember received topic and list member
		- Creator rest the creator if he quit the Channel ?


	QUIT: 
		- nbMember--
		- Channel destroy when the last client quit.

	topic: editable by operator
		-i invite only, only invited client can join
		- t topic protection, only operator can change the topic
		- k key, need password to join channel
		- o give and retire the operator privilege
		- l limit, limit the member to a max

	KICK:
		- operator only
		- Operator can't kick an other operator
	
	INVITE: operator only

	Member:
		- talk
		- quit canal
		- change topic if there isn't +t
*/

#endif