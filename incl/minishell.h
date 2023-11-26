/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:59:42 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/26 19:27:59 by aulicna          ###   ########.fr       */
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

// prompt.c
char	*set_prompt(char *env[]);

// ft_split_minishell.c
char	**ft_split_minishell(char const *s, char c);

#endif