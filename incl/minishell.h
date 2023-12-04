/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:59:42 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/29 15:26:25 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libftprintf/ft_printf.h"
# include <stdio.h>      // For printf, perror
# include <stdlib.h>     // For malloc, free, exit, getenv
# include <unistd.h>     // For read, write, access, close, fork, getcwd, chdir, execve, dup, dup2, pipe, isatty, ttyname, ttyslot
# include <fcntl.h>      // For open
# include <sys/wait.h>   // For wait, waitpid, wait3, wait4
# include <signal.h>     // For signal, sigaction, sigemptyset, sigaddset, kill
# include <sys/stat.h>   // For stat, lstat, fstat
# include <termios.h>    // For tcsetattr, tcgetattr
# include <dirent.h>     // For opendir, readdir, closedir
# include <string.h>     // For strerror
# include <sys/ioctl.h>  // For ioctl
# include <curses.h>     // For tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <readline/readline.h> // For readline-related functions
# include <readline/history.h>	// For readline-related functions

typedef enum s_tokens
{
	PIPE = 1,
	LESS,
	LESS_2,
	GREATER,
	GREATER_2,
}	t_tokens;

typedef enum s_builtins
{
	B_ECHO = 1,
	B_CD,
	B_PWD,
	B_EXPORT,
	B_UNSET,
	B_ENV,
	B_EXIT,
}	t_builtdins;

typedef struct s_lexer
{
	int		id;
	char	*word;
	t_tokens token;
}	t_lexer;

typedef struct s_simple_cmds
{
	char	**cmd;
	t_list	*redirects;
	t_builtdins	builtin;

}	t_simple_cmds;

// prompt.c
char	*set_prompt(char *env[]);

// input.c
int	check_quotes(char *input);

// ft_split_minishell.c
char	**ft_split_minishell(char const *s, char c);

// manage_quotes.c
void	count_qoutes(char c, unsigned int *s_quotes, unsigned int *d_quotes);
int	quotes_pair(unsigned int s_quotes, unsigned int d_quotes);

// lexer.c
t_list *input_arr_to_lexer_list(char **input_split);

// parser.c
t_list *lexer_to_simple_cmds(t_list **lexer);

#endif