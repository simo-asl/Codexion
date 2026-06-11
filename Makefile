CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
NAME = codexion
SRCS = coders/cleanup.c coders/coder_routine.c coders/monitor.c coders/main.c coders/dongles.c \
	coders/dongles_helper.c coders/heap.c coders/heap_utils.c coders/init.c \
	coders/runner.c coders/logs_state.c coders/time.c coders/ft_atol.c coders/parse.c coders/parse_utils.c
	
OBJS = $(SRCS:.c=.o)
HEADERS = coders/codexion.h coders/structs.h coders/prototypes.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

clean_all: fclean

re: fclean all

.PHONY: all clean fclean clean_all re
