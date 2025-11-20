#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "include.hpp"

/*
Forme canonique :
        T (); // Constructeur par défaut
        T (const T&); // Constructeur de recopie
        ~T (); // Destructeur éventuellement virtuel
        T &operator=(const T&); // Operator d'affectation
*/

class Command
{
	private :

	typedef void (Command::*FtCommand)(std::string, int);
	std::map<std::string, FtCommand> CommandMap;

	public :

	//Coplien form
	Command();
	Command(const Command&);
	~Command();
	Command& operator=(const Command&);

	//Methods to handle commands
	void	ft_redirect_buffer(std::string buffer, int client);

};

#endif