/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:59:42 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/05 19:01:20 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define PROMPT "minishell$ "

# define STDIN	0
# define STDOUT	1
# define STDERR	2

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

// ENVIRONMENT
int		env_init(char **envp, t_data *data);
int		env_add(t_list *head, char *env_var);
t_list	*env_find(t_list *head, char *variable_key);
t_list	**env_remove(t_list **head, char *variable_key);


// BUILTINS
int		ft_echo(char **args);
int		ft_pwd(void);
int		ft_env(t_data *data);
int		ft_cd(char **args, t_data *data);
char	*ft_cd_getpath(char *path_name, size_t len, t_data *data);
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
void	free_env(void *ptr);

int		free_array(char **strs);


#endif