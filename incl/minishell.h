/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:59:42 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/22 08:39:47 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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


#endif