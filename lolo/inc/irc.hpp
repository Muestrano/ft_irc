#pragma once

# include <stdlib.h>
# include <cstdio>
# include <unistd.h>
# include <iostream>
# include <iomanip>
# include <string>
# include <cstring>
# include <cstdlib>
# include <map>
# include <algorithm>
# include <signal.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sstream>
# include <signal.h>
# include <vector>
# include <netdb.h>
# include <poll.h>

/* SSTR
** @param x : the value
** @return the value in string
** Function to convert a value to a string
*/

#define SSTR( x ) static_cast< std::ostringstream & >( \
	( std::ostringstream() << std::dec << x ) ).str()

/*	IRC
**	@param ac : the number of arguments
**	@param av : the arguments
**	Launch the server
*/

void	ft_launch_server(char **av);
