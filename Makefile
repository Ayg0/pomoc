NAME=pomodoro

SRC = pomodoro.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)
fclean:	clean
	rm -f $(NAME)