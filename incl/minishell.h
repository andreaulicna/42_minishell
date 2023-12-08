/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:59:42 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/08 14:19:16 by aulicna          ###   ########.fr       */
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

typedef struct	s_data
{
	t_list	*env_list;
	t_list	*lexer;
	t_list	*simple_cmds;
	char	*prompt;
	char	*input;
	char	**input_split;
}				t_data;

typedef struct	s_env
{
	char		*name;
	char		*value;
	char		*full_string;
}				t_env;

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
}	t_builtins;

typedef struct s_lexer
{
	int			id;
	char		*word;
	t_tokens	token;
}	t_lexer;

typedef struct s_simple_cmds
{
	char		**cmd;
	t_list		*redirects;
	t_builtins	builtin;
}	t_simple_cmds;

// main.c
void	print_lexer(t_list **lexer);

// prompt.c
char	*set_prompt(char *env[]);

// input.c
int		check_quotes(char *input);

// ft_split_minishell.c
char	**ft_split_minishell(char const *s, char c);

// manage_quotes.c
void	count_qoutes(char c, unsigned int *s_quotes, unsigned int *d_quotes);
int		quotes_pair(unsigned int s_quotes, unsigned int d_quotes);

// lexer.c
t_list	*input_arr_to_lexer_list(char **input_split);

// parser.c
t_list	*lexer_to_simple_cmds(t_list **lexer);

// parser_redirects.c
void	separate_redirects(t_list **lexer, t_list **redirects);
void	free_lexer_node(t_list **lexer, int id);

// error.c
int		error_handler(int code);
int		error_parser_double_token(t_tokens token);

// expander.c
void expander(t_list **simple_cmds, t_data *data);

// free.c
void	free_lexer(t_list **lexer);
void	free_simple_cmds(t_list **simple_cmds);
void	free_data(t_data *data);

// env.c
int	env_init(char **envp, t_data *data);
t_list	*env_find(t_list *head, char *variable_key);
int	env_add(t_list **head, char *env_var);

#endif