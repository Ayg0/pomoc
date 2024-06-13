NAME=pomodoro


CC = gcc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g

CONF_FILE = '"./pomodoro.conf"'

SRC = $(shell find . -type f -name "*.c")
OBJ = $(SRC:.c=.o)

.c.o:
	$(CC) $(CFLAGS) -DCONF_FILE=$(CONF_FILE) -c $< -o $(<:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)
fclean:	clean
	rm -f $(NAME)
re: fclean all
