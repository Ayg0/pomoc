NAME=pomoc

CC = gcc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g

CONFIG_PATH = '"/home/orayne/.config/pomoc/pomoc.conf"'

SRC = $(shell find . -type f -name "*.c")
OBJ = $(SRC:.c=.o)

# uncomment ENABLE_LOG if you desire to see the parsing logs
DEFINES = -DCONF_FILE=$(CONFIG_PATH) #-DENABLE_LOG=1

.c.o:
	$(CC) $(CFLAGS) $(DEFINES) -c $< -o $(<:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)
fclean:	clean
	rm -f $(NAME)
re: fclean all
