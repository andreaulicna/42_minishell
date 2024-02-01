/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 22:14:18 by vbartos           #+#    #+#             */
/*   Updated: 2024/02/01 11:10:45 by vbartos          ###   ########.fr       */
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
void	orig_fds_save(int *orig_input, int *orig_output)
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
void	orig_fds_restore(int orig_input, int orig_output)
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
int	pipe_create(int fd_pipe[2])
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
int	pipe_close(int fd_pipe[2])
{
	close(fd_pipe[PIPE_WRITE]);
	return (fd_pipe[PIPE_READ]);
}

/**
 * @brief	Redirects input and output file descriptors for piping.
 * 
 * The write end of the current pipe must be closed after (possible) dupping
 * to ensure there is no fd left open in the child process.
 * 
 * @param	simple_cmd	current command being processed
 * @param	fd_pipe		2d array of file descriptors
 * @param	i			current position in fd_pipe
 */
void	pipe_redirect(t_list *simple_cmds, int **fd_pipe, int i)
{
	close(fd_pipe[i][PIPE_READ]);
	if (i > 0)
		dup2(fd_pipe[i - 1][PIPE_READ], STDIN_FILENO);
	if (simple_cmds->next != NULL)
		dup2(fd_pipe[i][PIPE_WRITE], STDOUT_FILENO);
	close(fd_pipe[i][PIPE_WRITE]);
}
