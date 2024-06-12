NAME=pomodoro

SRC = pomodoro.c

CFLAGS = -Wall -Wextra -Werror -g

CATCOLOR = \"\\033[33m\"

OBJ = $(SRC:.c=.o)

.c.o:
	$(CC) $(CFLAGS) -DCATCOLOR=$(CATCOLOR) -c $< -o $(<:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)
fclean:	clean
	rm -f $(NAME)
re: fclean all
