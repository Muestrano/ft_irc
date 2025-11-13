# FT_IRC # Internet relay chat between a client and server

NAME = ircserv
FLAGS = -Wall -Werror -Wextra -std=c++98
CC = c++

# Sources Files
SRCS_FOLDER = srcs/
SRCS = srcs/main.cpp srcs/Server/Server.cpp srcs/Client/Client.cpp srcs/Server/Parsing.cpp srcs/Server/Utils.cpp srcs/Channel/Channel.cpp 
OBJS = $(SRCS:.cpp=.o)

# Colorsq
GREEN	=	\033[0;32m
RED		=	\033[0;31m
RESET	=	\033[0m

# Rules
all:	$(NAME)

$(NAME):	$(OBJS)
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled succesfully!$(RESET)"

%.o:	%.cpp
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@echo "$(RED)$(RED) objects deleted successfully!$(RESET)"

fclean:	clean
	@rm -f $(NAME)
	@echo "$(RED)$(NAME) deleted successfully!$(RESET)"

re:	fclean all

.PHONY:	all clean fclean re
