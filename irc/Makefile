# *********** Color Definition ************ #
RED = \033[91m
YELLOW = \033[33m
GREEN = \033[92m
# **************************************** #

NAME = ircserv
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g

DIR_SRC = src/
DIR_OBJ = obj/
DIR_INCLUDE = include/

RM = rm -f

SRCS := main.cpp channel.cpp client.cpp server.cpp
OBJS := $(addprefix $(DIR_OBJ), $(SRCS:.cpp=.o))

INCLUDES = -I$(DIR_INCLUDE)

all: $(NAME)

$(DIR_OBJ)%.o: $(DIR_SRC)%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	@echo "$(YELLOW)...Compiling..."
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(GREEN)Compilation OK ✅"

clean:
	@rm -rf $(DIR_OBJ)
	@echo "$(RED)Deleting object files"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED)Deleting executable"

re: fclean all

.PHONY: all clean fclean re
