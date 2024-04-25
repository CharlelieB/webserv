NAME := webserv
CC := c++
CFLAGS := -Wall -Wextra -Werror -std=c++98
SRCS = main.cpp
OBJ_DIR := obj
OBJS := $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
HEADERS := *.hpp
DIR_DUP = mkdir -p $(@D)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp $(HEADERS)
	@$(DIR_DUP)
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
