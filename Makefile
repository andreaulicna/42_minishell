# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/22 08:49:17 by vbartos           #+#    #+#              #
#    Updated: 2023/12/06 10:51:05 by vbartos          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC = main/minishell.c\
		main/free.c\
		main/utils.c\
		main/env.c\
		builtins/ft_echo.c\
		builtins/ft_pwd.c\
		builtins/ft_env.c\
		builtins/ft_cd.c\
		builtins/ft_export.c\
		builtins/ft_unset.c\
		builtins/ft_exit.c

OBJ = $(SRC:.c=.o)

HEADER = inc/minishell.h

CFLAGS = -Wall -Werror -Wextra -g

GCC = gcc

LIBFTPRINTF = lib

all: libs $(NAME)
	@echo "minishell executable ready ✅"

.c.o:
	@$(GCC) $(CFLAGS) -c $< -o $@

libs:
	@make -C $(LIBFTPRINTF)
	@echo "libprintf library ready ✅"

$(NAME): $(OBJ) $(HEADER)
	@$(GCC) $(CFLAGS) -L $(LIBFTPRINTF) -o $@ $^ -lreadline -lftprintf

clean:
	@rm -f $(OBJ)
	@make clean -C $(LIBFTPRINTF)

fclean:
	@rm -f $(OBJ)
	@rm -f $(NAME)
	@make fclean -C $(LIBFTPRINTF)

re: fclean all

.PHONY: all clean fclean re