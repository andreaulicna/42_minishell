/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 22:14:18 by vbartos           #+#    #+#             */
/*   Updated: 2024/01/08 13:05:28 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * Saves the original file descriptors for standard input and standard output.
 *
 * @param orig_input Pointer to store the original standard input file
 * descriptor.
 * @param orig_output Pointer to store the original standard output file
 * descriptor.
 */
void orig_fds_save(int *orig_input, int *orig_output)
{
	*orig_output = dup(STDOUT);
	*orig_input = dup(STDIN);
}

/**
 * Restores the original file descriptors for standard input and standard
 * output.
 * 
 * @param orig_input The original file descriptor for standard input.
 * @param orig_output The original file descriptor for standard output.
 */
void orig_fds_restore(int orig_input, int orig_output)
{
	dup2(orig_output, STDOUT);
	dup2(orig_input, STDIN);
	close(orig_input);
	close(orig_output);
}

/**
 * Creates a pipe using the pipe system call.
 * 
 * @param fd_pipe An array to store the file descriptors for the pipe.
 * @return The write end file descriptor of the pipe.
 */
int pipe_create(int fd_pipe[2])
{
	if (pipe(fd_pipe) == -1)
	{
		ft_putendl_fd("minishell: pipe: Too many open files", 2);
		exit_current_prompt(NULL);
	}
	return (fd_pipe[PIPE_WRITE]);
}

/**
 * Closes the write end of a pipe and returns the read end.
 *
 * @param fd_pipe The file descriptors for the pipe.
 * @return The read end of the pipe.
 */
int pipe_close(int fd_pipe[2])
{
	close(fd_pipe[PIPE_WRITE]);
	return (fd_pipe[PIPE_READ]);
}

/**
 * Redirects input and output file descriptors for piping.
 *
 * @param fd_input The file descriptor for input.
 * @param fd_output The file descriptor for output.
 */
void pipe_redirect(int fd_input, int fd_output)
{
	if (fd_input != STDIN)
	{
		dup2(fd_input, STDIN);
		close(fd_input);
	}
	if (fd_output != STDOUT)
	{
		dup2(fd_output, STDOUT);
		close(fd_output);
	}
}