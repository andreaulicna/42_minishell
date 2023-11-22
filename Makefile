# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/20 11:55:38 by aulicna           #+#    #+#              #
#    Updated: 2023/11/22 11:12:19 by aulicna          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = simple_shell

SRC = main.c

OBJ = $(SRC:.c=.o)

HEADER = simple_shell

CFLAGS = -Wall -Werror -Wextra

GCC = gcc

LIBFTPRINTF = libftprintf

all: libs $(NAME)
	@echo "simple_shell executable ready ✅"

.c.o:
	$(GCC) $(CFLAGS) -c $< -o $@

libs:
	@make -C $(LIBFTPRINTF)
	@echo "libprintf library ready ✅"

$(NAME): $(OBJ) $(HEADER)
	$(GCC) $(CFLAGS) -L $(LIBFTPRINTF) -o $@ $^ -lreadline -lftprintf

clean:
	@rm -f $(OBJ)
	@make clean -C $(LIBFTPRINTF)

fclean:
	@rm -f $(OBJ)
	@rm -f $(NAME)
	@make fclean -C $(LIBFTPRINTF)

re: fclean all

.PHONY: all clean fclean re