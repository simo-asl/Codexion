NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRC = coders/main.c \
	coders/parse.c \
	coders/parse_utils.c \
	coders/init.c \
	coders/init_utils.c \
	coders/simulation.c \
	coders/coder_routine.c \
	coders/dongles.c \
	coders/scheduler.c \
	coders/heap.c \
	coders/monitor.c \
	coders/logging.c \
	coders/time_utils.c \
	coders/cleanup.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
