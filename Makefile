NAME=pomodoro

SRC = pomodoro.c parseConfigFile.c

CFLAGS = -Wall -Wextra -Werror -g

CONF_FILE = '"./pomodoro.conf"'

OBJ = $(SRC:.c=.o)

.c.o:
	$(CC) $(CFLAGS) -DCONF_FILE=$(CONF_FILE) -c $< -o $(<:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)
fclean:	clean
	rm -f $(NAME)
re: fclean all
