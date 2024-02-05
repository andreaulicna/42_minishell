/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 22:14:18 by vbartos           #+#    #+#             */
/*   Updated: 2024/02/05 17:35:31 by aulicna          ###   ########.fr       */
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
 */
void	pipe_create(int **fd_pipe, int i)
{
	if (pipe(fd_pipe[i]) == -1)
	{
		ft_putendl_fd("minishell: pipe: Too many open files", 2);
		free_pipe_child(fd_pipe, i);
		exit_current_prompt(NULL);
	}
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
	t_simple_cmds	*content;

	close(fd_pipe[i][PIPE_READ]);
	if (i > 0)
		dup2(fd_pipe[i - 1][PIPE_READ], STDIN_FILENO);
	if (simple_cmds->next != NULL)
		dup2(fd_pipe[i][PIPE_WRITE], STDOUT_FILENO);
	content = (t_simple_cmds *) simple_cmds->content;
	if (i > 0 && content->cmd[0] == NULL)
		close(fd_pipe[i - 1][PIPE_READ]);
	close(fd_pipe[i][PIPE_WRITE]);
}

/**
 * @brief	Waits for all processes in the pipeline to finish.
 * 
 * The if and else statement after while loop ensure there is no fd left open
 * in the parent process for one command and piped commands, respectively.
 * 
 * The last if handles the case of a hanging command (e.g. cat or sort) getting
 * interrupted by SIGINT as g_signal == SIGUSR2 indicates that there was
 * such a command and therefore the exit_status should be set to 130.
 *
 * @param	data		pointer to the t_data structure (for exit_status)
 * @param	cmds_num	number of commands in the pipeline
 * @param	fd_pipe		2d array of file descriptors
 * @param	i			current position in fd_pipe
 */
int	wait_for_pipeline(int cmds_num, int **fd_pipe, int i, int pid_list[])
{
	int	j;
	int	status;
	int	exit_status;

	j = 0;
	while (j < cmds_num)
	{
		waitpid(pid_list[j], &status, 0);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exit_status = signal_exit_of_child(&status);
		j++;
	}
	if (cmds_num > 0)
		close(fd_pipe[i - 1][PIPE_READ]);
	if (cmds_num != 1)
		close(fd_pipe[i - 2][PIPE_WRITE]);
	return (exit_status);
}
