/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:59:42 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/14 18:41:30 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

# define STDIN	0
# define STDOUT	1
# define STDERR	2

# define PIPE_READ	0
# define PIPE_WRITE	1

# define EXIT_MALLOC		4
# define EXIT_EXECVE 		5
# define EXIT_EXECNOTFOUND	127

typedef struct s_data
{
	t_list	*env_list;
	t_list	*lexer;
	t_list	*simple_cmds;
	char	*prompt;
	char	*input;
	char	**input_split;
	int		orig_fdin;
	int		orig_fdout;
	int		exit_status;
}				t_data;

typedef struct s_env
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
	char		*hd_file;
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
// main_a.c
int		minishell_loop(t_data *data);
// print.c
void	print_input_split(char **input_split);
void	print_lexer(t_list **lexer);
void	print_simple_cmds(t_list **simple_cmds);

/* Error */
// error.c
int		error_handler(int code);
int		error_parser_double_token(t_tokens token);

/* Exit */
// exit.c
void	exit_minishell(t_data *data, int exit_status);
void	exit_current_prompt(t_data *data);
// free.c
void	free_data(t_data *data);
void	free_data_current_prompt(t_data *data);
// free_helpers.c
int		free_array(char **strs);
void	free_lexer(t_list **lexer);
void	free_simple_cmds(t_list **simple_cmds);
int		free_envlist(t_list **head);
void	free_struct_str(t_str *str);

/* Expander */
// expander.c
void	expander(t_data *data);
int		contains_dollar(char *str);
void	expander_loop_dollar(t_simple_cmds *content, int i, t_data *data);
// expander_dollar.c
int		checker_dollar(char *str, int j);
void	expand_exit_status(char **cmd, int i_cmd, int exit_status);
void	expand_dollar(char **cmd, int i_cmd, t_list *env_list, int *j_cmd);
void	delete_backslash(char **cmd, int i_cmd);

/* Heredoc */
int		heredoc(t_data *data);
void	heredoc_no_space(t_data *data);

/* Lexer */
// ft_split_minishell.c
char	**ft_split_minishell(char const *s, char c);
//	no_space_split.c
int		contains_space(char *s);
t_tokens	contains_token_with_no_space(char *s);
char	**no_space_split(char **input_split, int index, t_tokens token);
// lexer.c
t_tokens	is_token(char *check);
int		input_arr_to_lexer_list(t_data *data);

/* Parser */
// parser.c
int		lexer_to_simple_cmds(t_list **lexer, t_list **simple_cmds);
// parser_redirects.c
void	separate_redirects(t_list **lexer, t_list **redirects);
void	free_lexer_node(t_list **lexer, int id);

// env.c
int		env_init(char **envp, t_data *data);
t_list	*env_find(t_list *head, char *variable_key);
int		env_add(t_list **head, char *env_var);
t_list	**env_remove(t_list **head, char *variable_key);
size_t	strs_count(char **strs);

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

/* Builtins */
void	ft_echo(char **args, t_data *data);
void	ft_pwd(t_data *data);
void	ft_env(t_data *data);
void	ft_cd(char **args, t_data *data);
char	*ft_cd_getpath(char *path_name, t_data *data);
void	ft_cd_update(char *oldpwd, t_data *data);
void	ft_cd_home(char *oldpwd, t_data *data);
void	ft_cd_previous(char *oldpwd, t_data *data);
void	ft_export(char **args, t_data *data);
void	ft_export_add(char **args, t_data *data, int i);
void	ft_export_list(t_data *data);
void	ft_export_sort(char **env_arr);
void	ft_export_format(char *env_var);
int		ft_export_validate(char *arg);
void	ft_unset(char **args, t_data *data);
void	ft_exit(char **args, t_data *data);
int		ft_exit_checknum(char *str);

/* Execution */
int		exec(t_data *data, t_list *simple_cmds);
void	exec_pipeline(t_data *data, t_list *simple_cmds, int cmds_num);
int		fork_cmd(t_data *data, t_list *simple_cmds, int fd_input, int fd_output);
void	run_builtin(t_data *data, char **cmd);
void	run_exec(t_data *data, t_simple_cmds *content);
char	*find_exe_path(t_data *data, char *cmd);
int		is_builtin(char *cmd);
void	handle_redirect(t_list *redirects, char *hd_file);
void	handle_output_single(char *filename);
void	handle_output_append(char *filename);
void	handle_input(char *filename);
char	**env_copy(t_data *data);
void	wait_for_pipeline(t_data *data, int pid_list[], int cmds_num);
int		pipe_create(int fd_pipe[2]);
int		pipe_close(int fd_pipe[2]);
void	pipe_redirect(int fd_input, int fd_output);
void	orig_fds_save(int *orig_input, int *orig_output);
void	orig_fds_restore(int orig_input, int orig_output);

/* Signals */
void	init_signals(void);
void	handle_sigint(int sig_num);

#endif