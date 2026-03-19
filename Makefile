# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mel-asla <mel-asla@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/19 09:33:01 by mel-asla          #+#    #+#              #
#    Updated: 2026/03/19 09:33:10 by mel-asla         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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
