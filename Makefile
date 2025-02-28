NAME = microshell
CC = gcc
FILE = microshell.c
FLAGS = -Wextra -Werror -Wall

all : $(NAME)

# $(NAME): $(FILE) Makefile
# 	$(CC) $(FLAGS) $(FILE) -g -o $(NAME)

$(NAME): $(FILE) Makefile
	$(CC) $(FILE) -g -o $(NAME)

clean:
	rm $(NAME)