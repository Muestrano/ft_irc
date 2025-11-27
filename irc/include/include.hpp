#ifndef IRC_HPP
# define IRC_HPP

#include <string>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> //sockaddr_in
#include <fcntl.h>
#include <vector>
#include <poll.h>
#include <cerrno>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <map>
#include <algorithm>
#include <err.h>

#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "Server.hpp"

// TEMP : Sorted by numeric values.

enum ErrorCode 
{
    ERR_UNKNOWNCOMMAND = 421,
    ERR_NONICKNAMEGIVEN = 431,
    ERR_ERRONEUSNICKNAME = 432,
    ERR_NICKNAMEINUSE = 433,
    ERR_NICKCOLLISION = 436
};

class Commands;


#endif