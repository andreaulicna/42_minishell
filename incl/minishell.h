/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:59:42 by aulicna           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/12/12 12:34:57 by vbartos          ###   ########.fr       */
=======
/*   Updated: 2023/12/12 13:45:13 by aulicna          ###   ########.fr       */
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

<<<<<<< HEAD
# define PROMPT "minishell$ "

# define STDIN	0
# define STDOUT	1
# define STDERR	2

# define EXIT_MALLOC	4

// IMPORT FUNCTIONS
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

// IMPORT MACROS
// # include <limits.h> 	// For PATH_MAX
# include <sys/param.h> // For PATH_MAX (different systems)

// IMPORT CUSTOM LIBRARIES
# include "../lib/ft_printf.h"

// STRUCTS
typedef struct	s_data
{
	t_list		*env_list;
}				t_data;

typedef struct	s_env
=======
# include "../libftprintf/ft_printf.h"
# include <stdio.h>      // printf, perror
# include <stdlib.h>     // malloc, free, exit, getenv
# include <unistd.h>     // access, fork, getcwd, chdir, execve, dup(2), pipe
# include <fcntl.h>      // open
# include <sys/wait.h>   // wait, waitpid, wait3, wait4
# include <signal.h>     // signal, sigaction, sigemptyset, sigaddset, kill
# include <sys/stat.h>   // stat, lstat, fstat
# include <termios.h>    // tcsetattr, tcgetattr
# include <dirent.h>     // opendir, readdir, closedir
# include <string.h>     // strerror
# include <sys/ioctl.h>  // ioctl
# include <curses.h>     // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <readline/readline.h> // For readline-related functions
# include <readline/history.h>	// For readline-related functions

# define EXIT_MALLOC 4

typedef struct s_data
{
	t_list	*env_list;
	t_list	*lexer;
	t_list	*simple_cmds;
	char	*prompt;
	char	*input;
	char	**input_split;
}				t_data;

typedef struct s_env
>>>>>>> main
{
	char		*name;
	char		*value;
	char		*full_string;
}				t_env;

<<<<<<< HEAD
// ENVIRONMENT
int		env_init(char **envp, t_data *data);
int		env_add(t_list **head, char *env_var);
t_list	*env_find(t_list *head, char *variable_key);
t_list	**env_remove(t_list **head, char *variable_key);

// BUILTINS
int		ft_echo(char **args);
int		ft_pwd(void);
int		ft_env(t_data *data);
int		ft_cd(char **args, t_data *data);
char	*ft_cd_getpath(char *path_name, t_data *data);
void	ft_cd_update(char *oldpwd, t_data *data);
int		ft_cd_home(char *oldpwd, t_data *data);
int		ft_cd_previous(char *oldpwd, t_data *data);
int		ft_export(char **args, t_data *data);
void	ft_export_list(t_data *data);
void	ft_export_sort(char **env_arr);
void	ft_export_format(char *env_var);
int		ft_unset(char **args, t_data *data);
void	ft_exit(char **args, t_data *data);
int		ft_exit_checknum(char *str);

// UTILS
int		init_data(t_data *data, char **envp);
size_t	strs_count(char **strs);
void	exit_minishell(t_data *data, int exit_status);

// FREES
void	free_data(t_data *data);
int		free_envlist(t_list **head);
int		free_array(char **strs);

=======
typedef enum s_tokens
{
	PIPE = 1,
	LESS,
	LESS_2,
	GREATER,
	GREATER_2,
}	t_tokens;

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
}	t_simple_cmds;

typedef struct s_str
{
	char	*part_1;
	char	*part_2;
	char	*part_3;
	char	*tmp_join;
	char	*final;
	t_list	*env_found;
	t_env	*content;
}	t_str;

/* SOURCES */

/* Debug */
// print.c
void	print_lexer(t_list **lexer);
void	print_simple_cmds(t_list **simple_cmds);

/* Error */
// error.c
int		error_handler(int code);
int		error_parser_double_token(t_tokens token);

/* Exit */
// free.c
void	free_data(t_data *data);
// free_helpers.c
int		free_array(char **strs);
void	free_lexer(t_list **lexer);
void	free_simple_cmds(t_list **simple_cmds);
int		free_envlist(t_list **head);
void	free_struct_str(t_str *str);

/* Expander */
// expander.c
void	expander(t_data *data);
void	init_struct_str(t_str *str);
// expander_dollar.c
void	expander_loop_dollar(t_simple_cmds *content, int i, t_list *env_list);

/* Lexer */
// ft_split_minishell.c
char	**ft_split_minishell(char const *s, char c);
// lexer.c
t_list	*input_arr_to_lexer_list(char **input_split);

/* Parser */
// parser.c
t_list	*lexer_to_simple_cmds(t_list **lexer);
// parser_redirects.c
void	separate_redirects(t_list **lexer, t_list **redirects);
void	free_lexer_node(t_list **lexer, int id);

// env.c
int		env_init(char **envp, t_data *data);
t_list	*env_find(t_list *head, char *variable_key);
int		env_add(t_list **head, char *env_var);

// init.c
void	init_data(t_data *data);
void	init_struct_str(t_str *str);

// prompt.c
char	*set_prompt(t_list *env_list);

// quotes.c
int		check_quotes(char *input);
void	delete_quotes(char **cmd, int i_cmd);
void	count_qoutes(char c, unsigned int *s_quotes, unsigned int *d_quotes);
int		quotes_pair(unsigned int s_quotes, unsigned int d_quotes);
>>>>>>> main

#endif