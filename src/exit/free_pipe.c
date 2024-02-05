/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 13:17:14 by aulicna           #+#    #+#             */
/*   Updated: 2024/02/05 00:57:05 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief	Frees all the malloced pipes after executing the whole pipeline.
 * 
 * @param	fd_pipe		array of pipes to free
 * @param	num_cmds	number of commands (lstsize simple_cmds) in the pipeline
*/
void	free_pipe(int **fd_pipe, int num_cmds)
{
	int	i;

	i = 0;
	while (i < num_cmds)
	{
		free(fd_pipe[i]);
		i++;
	}
	free(fd_pipe);
}

/**
 * @brief	Frees malloced pipes in the child process. That includes all
 * the pipes up until and including the current one.
 * 
 * The read end of the second to last pipe (from the child process's point
 * of view) is closed as not to leave an open file descriptor upon child
 * process exit (e.g. when the content of the pipe was ignored by the command).
 * 
 * @param	fd_pipe		array of pipes to free
 * @param	i			index of the current pipe
*/
void	free_pipe_child(int **fd_pipe, int i)
{
	int	j;

	if (i > 0)
	{
		close(fd_pipe[i - 1][PIPE_READ]);
		j = 0;
		while (j < i)
		{
			free(fd_pipe[j]);
			j++;
		}
	}
	free(fd_pipe[i]);
	free(fd_pipe);
}
