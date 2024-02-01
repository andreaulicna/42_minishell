# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/23 14:30:35 by aulicna           #+#    #+#              #
#    Updated: 2024/02/01 16:05:59 by aulicna          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC = main.c\
		src/debug/print.c\
		src/setup/init.c\
		src/setup/main_checkers.c\
		src/setup/prompt.c\
		src/utils/utils.c\
		src/utils/env.c\
		src/utils/signals.c\
		src/builtins/builtins_utils.c\
		src/builtins/ft_echo.c\
		src/builtins/ft_pwd.c\
		src/builtins/ft_env.c\
		src/builtins/ft_cd.c\
		src/builtins/ft_export.c\
		src/builtins/ft_unset.c\
		src/builtins/ft_exit.c\
		src/exit/exit.c\
		src/exit/free.c\
		src/exit/free_helpers.c\
		src/exit/free_pipe.c\
		src/expander/expander.c\
		src/expander/expander_checkers.c\
		src/expander/expander_construct.c\
		src/expander/expander_empty_env.c\
		src/expander/quotes_delete.c\
		src/heredoc/heredoc.c\
		src/heredoc/heredoc_lines.c\
		src/lexer/ft_split_minishell.c\
		src/lexer/lexer.c\
		src/lexer/no_space_split.c\
		src/lexer/quotes_split.c\
		src/parser/parser.c\
		src/parser/parser_redirects.c\
		src/parser/open_pipe.c\
		src/exec/exec.c\
		src/exec/exec_utils.c\
		src/exec/pipe_utils.c\
		src/exec/redirects_utils.c

OBJ = $(SRC)
OBJ := $(OBJ:%.c=%.o)

HEADER = -I ./incl/

CFLAGS = -Wall -Werror -Wextra -g

CC = cc

LIBFTPRINTF = libftprintf

all: libs $(NAME)
	@find ~/ -name ".tmp_heredoc_*" -type f -delete
	@echo "minishell executable ready ✅"

%.o: %.c
	$(CC) $(CFLAGS) -c $< $(HEADER) -o $@

libs:
	@make -C $(LIBFTPRINTF)
	@mv $(LIBFTPRINTF)/$(LIBFTPRINTF).a .
	@echo "libprintf library ready ✅"

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFTPRINTF).a -o $(NAME) -lreadline

clean:
	@rm -f $(OBJ)
	@find ~/ -name ".tmp_heredoc_*" -type f -delete
	@make clean -C $(LIBFTPRINTF)

fclean: clean
	@rm -f $(NAME)
	@rm -f libftprintf.a
	@make fclean -C $(LIBFTPRINTF)

re: fclean all

.PHONY: all clean fclean re