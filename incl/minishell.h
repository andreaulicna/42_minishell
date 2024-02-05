/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:59:42 by aulicna           #+#    #+#             */
/*   Updated: 2024/02/05 00:22:49 by aulicna          ###   ########.fr       */
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

extern int	g_signal;

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
	int		hd_fd;
	int		*pid_list;
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
// main.c
int		minishell_loop(t_data *data);
// print.c
void	print_input_split(char **input_split);
void	print_lexer(t_list **lexer);
void	print_simple_cmds(t_list **simple_cmds);

/* Setup */
// main_checkers.c
int		check_quotes(char *input);
int		check_input_null(char *input);
int		check_enter_space(char *input);
// init.c
void	init_data(t_data *data);
void	init_struct_str(t_str *str);
// prompt.c
char	*set_prompt(t_list *env_list);

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
void	free_struct_str(t_str *str, char *old_str);
// free_pipe.c
void	free_pipe(int **fd_pipe, int num_cmds);
void	free_pipe_child(int **fd_pipe, int i);

/* Expander */
// expander.c
void	expander(t_data *data);
void	expander_loop_dollar(t_simple_cmds *content, int i, int exit_status,
			t_list *env_list);
void	expander_loop_dollar_no_space(t_data *data, t_simple_cmds *content,
			int i);
// expander_dollar_checkers.c
int		contains_dollar(char *str);
int		checker_dollar(char *str, int j);
int		env_in_quotes_followed(char *str);
// expander_construct.c
char	*expand_exit_status(char *str, int exit_status);
char	*expand_dollar(char *str, t_list *env_list, int *j_cmd);
char	*delete_backslash(char *str);
//	expander_emtpy_env.c
char	**ft_strdup_array(char **arr);
void	handle_empty_envs(char **old_cmd, t_simple_cmds *content,
			int *exit_status);
// quotes_delete.c
int		get_quotes_type(char *str, char *q);
char	*delete_quotes(char *str);
int		has_quotes_to_delete(char *str);

/* Heredoc */
// heredoc.c
int		heredoc(t_data *data);
// heredoc_lines.c
void	create_heredoc(t_list *heredoc, char *hd_file_name, t_data *data);

/* Lexer */
// ft_split_minishell.c
char	**ft_split_minishell(char const *s, char c);
// lexer.c
int		input_arr_to_lexer_list(t_data *data);
//	no_space_split.c
char	**no_space_split(char **input_split, int index);
// quotes.c
void	count_qoutes(char c, unsigned int *s_quotes, unsigned int *d_quotes);
int		quotes_pair(unsigned int s_quotes, unsigned int d_quotes);

/* Parser */
// parser.c
int		error_parser_double_token(t_tokens token);
int		lexer_to_simple_cmds(t_list **lexer, t_list **simple_cmds);
// parser_redirects.c
void	separate_redirects(t_list **lexer, t_list **redirects);
void	free_lexer_node(t_list **lexer, int id);
// open_pipe.c
void	handle_open_pipe(t_data *data);

// env.c
int		env_init(char **envp, t_data *data);
t_list	*env_find(t_list *head, char *variable_key);
int		env_add(t_list **head, char *env_var);
t_list	**env_remove(t_list **head, char *variable_key);
size_t	strs_count(char **strs);

/* Builtins */
void	ft_echo(char **args, t_data *data);
void	ft_pwd(t_data *data);
void	ft_env(t_data *data);
void	ft_cd(char **args, t_data *data);
void	ft_cd_nosuchfile(char *arg);
void	ft_cd_toomanyargs(t_data *data);
void	ft_export(char **args, t_data *data);
int		ft_export_validate(char *arg);
void	ft_unset(char **args, t_data *data);
void	ft_exit(char **args, t_data *data);
int		ft_exit_checknum(char *str);

/* Execution */
int		exec(t_data *data, t_list *simple_cmds);
void	exec_pipeline(t_data *data, t_list *simple_cmds, int cmds_num);
int		fork_cmd(t_data *data, t_list *simple_cmds, int **new_pipe, int i);
void	run_builtin(t_data *data, char **cmd);
void	run_exec(t_data *data, t_simple_cmds *content);
char	*find_exe_path(t_data *data, char *cmd);
int		is_builtin(char *cmd);
void	handle_redirect(t_list *redirects, char *hd_file);
void	check_for_files(t_simple_cmds *content, char **env_cpy);
char	**env_copy(t_data *data);
int		wait_for_pipeline(int cmds_num, int **fd_pipe, int i, int pid_list[]);
void	pipe_create(int **fd_pipe, int i);
void	pipe_redirect(t_list *simple_cmds, int **fd_pipe, int i);
void	orig_fds_save(int *orig_input, int *orig_output);
void	orig_fds_restore(int orig_input, int orig_output);
void	fork_process(int *pid);

/* Signals */
void	handle_sigint(int sig_num);
void	handle_sigint_heredoc(int sig_num);
void	handle_sigint_hanging_command(int sig_num);

#endif