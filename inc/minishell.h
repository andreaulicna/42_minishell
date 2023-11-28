/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:59:42 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/28 13:18:45 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define PROMPT "minishell$ "

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
# include <limits.h>	// For PATH_MAX

// IMPORT CUSTOM LIBRARIES
# include "../lib/ft_printf.h"
# include "../lib/libft/libft.h"
# include "../lib/libft/get_next_line_bonus.h"

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
int		ft_env(char **envp);
int		ft_cd(char **args);

// UTILS
size_t	strs_count(char **strs);
int		free_list(char **strs);

#endif