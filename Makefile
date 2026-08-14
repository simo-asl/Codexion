NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
SRC = coders/main.c coders/init.c \
	 coders/cleanup.c coders/simulation.c coders/coder.c coders/coder_start.c \
	 coders/log.c coders/dongles.c coders/dongle_pair.c coders/heap.c \
	 coders/heap_ops.c coders/monitor.c coders/time.c
OBJ = $(SRC:.c=.o)
HDR = coders/codexion.h

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
