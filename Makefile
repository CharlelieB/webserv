NAME := webserv
CC := c++
CFLAGS := -Wall -Wextra -Werror -g3 -std=c++98
OBJ_DIR := obj
SRC_DIR := src
SRCS = main.cpp Conf.cpp Lexer.cpp Parser.cpp Route.cpp VirtualServer.cpp Request.cpp ServerManager.cpp Client.cpp
SRCS := $(SRCS:%=$(SRC_DIR)/%)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
HEADERS := src/*.hpp
DIR_DUP = mkdir -p $(@D)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@$(DIR_DUP)
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
