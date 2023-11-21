# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/20 11:55:38 by aulicna           #+#    #+#              #
#    Updated: 2023/11/21 12:17:43 by aulicna          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = simple_shell

SRC = main.c

OBJ = $(SRC:.c=.o)

HEADER = simple_shell

CFLAGS = -Wall -Werror -Wextra

GCC = gcc

all: $(NAME)
	@echo "simple_shell executable ready ✅"

.c.o:
	$(GCC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ) $(HEADER)
	$(GCC) $(CFLAGS) -o $@ $^ -lreadline

clean:
	@rm -f $(OBJ)

fclean:
	@rm -f $(OBJ)
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re