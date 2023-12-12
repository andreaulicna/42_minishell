# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
<<<<<<< HEAD
#    By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/22 08:49:17 by vbartos           #+#    #+#              #
#    Updated: 2023/12/12 15:27:29 by vbartos          ###   ########.fr        #
=======
#    By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/23 14:30:35 by aulicna           #+#    #+#              #
#    Updated: 2023/12/12 13:48:26 by aulicna          ###   ########.fr        #
>>>>>>> main
#                                                                              #
# **************************************************************************** #

NAME = minishell

<<<<<<< HEAD
SRC = src/debug/test-v1.c\
		src/exit/exit.c\
		src/exit/free.c\
		src/utils/utils.c\
		src/utils/env.c\
		src/builtins/ft_echo.c\
		src/builtins/ft_pwd.c\
		src/builtins/ft_env.c\
		src/builtins/ft_cd.c\
		src/builtins/ft_export.c\
		src/builtins/ft_unset.c\
		src/builtins/ft_exit.c
=======
SRC = src/debug/main_a.c \
						src/debug/print.c \
						src/error/error_token.c \
						src/exit/free.c \
						src/exit/free_helpers.c \
						src/expander/expander.c \
						src/expander/expander_dollar.c \
						src/lexer/ft_split_minishell.c \
						src/lexer/lexer.c \
						src/parser/parser_redirects.c \
						src/parser/parser.c \
						src/init.c \
						src/prompt.c \
						src/quotes.c
>>>>>>> main

OBJ = $(SRC:.c=.o)

HEADER = incl/minishell.h

CFLAGS = -Wall -Werror -Wextra -g

GCC = gcc

<<<<<<< HEAD
LIBFTPRINTF = lib
=======
LIBFTPRINTF = libftprintf
>>>>>>> main

all: libs $(NAME)
	@echo "minishell executable ready ✅"

.c.o:
<<<<<<< HEAD
	@$(GCC) $(CFLAGS) -c $< -o $@
=======
	$(GCC) $(CFLAGS) -c $< -o $@
>>>>>>> main

libs:
	@make -C $(LIBFTPRINTF)
	@echo "libprintf library ready ✅"

$(NAME): $(OBJ) $(HEADER)
<<<<<<< HEAD
	@$(GCC) $(CFLAGS) -L $(LIBFTPRINTF) -o $@ $^ -lreadline -lftprintf
=======
	$(GCC) $(CFLAGS) -L $(LIBFTPRINTF) -o $@ $^ -lreadline -lftprintf
>>>>>>> main

clean:
	@rm -f $(OBJ)
	@make clean -C $(LIBFTPRINTF)

fclean:
	@rm -f $(OBJ)
	@rm -f $(NAME)
	@make fclean -C $(LIBFTPRINTF)

re: fclean all

.PHONY: all clean fclean re