/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 11:33:30 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/23 12:42:44 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

// get a number of commands
// - for each command, create a child process
int	exec(t_data *data, t_list *simple_cmds)
{
	size_t			cmd_num;

	cmd_num = ft_lstsize(simple_cmds);
	if (cmd_num == 1)
		exec_singlecmd(data, simple_cmds);
	else if (cmd_num > 1)
		// exec_multicmds(data, simple_cmds, cmd_num);
		return (1);
	return (0);
}

void exec_singlecmd(t_data *data, t_list *simple_cmds)
{
	pid_t			pid;
	t_simple_cmds	*content;
	char			*path;
	char			**env_copy;

	env_copy = exec_copyenv(data);
	pid = fork();
	if (pid == -1)
	{
		free(env_copy);
		exit_current_prompt(NULL);
	}
	content = (t_simple_cmds *) simple_cmds->content;
	if (pid == 0)
	{
		if (content->redirects)
			exec_handleredirect(content->redirects);
		if (exec_isbuiltin(content->cmd[0]))
			exec_runbuiltin(data, content->cmd);
		else
		{
			path = exec_findpath(data, content->cmd[0]);
			if (path != NULL)
			{
				execve(path, content->cmd, env_copy);
				free(path);
			}
			free(env_copy);
			exit_current_prompt(NULL);
		}
	}
	else
	{
		waitpid(pid, NULL, 0);
		free(env_copy);
	}
}

// void exec_multicmds(t_data *data, t_list *simple_cmds, size_t cmd_num)
// {
// 	int	tmpin = dup(STDIN);
// 	int	tmpout = dup(STDOUT);
// 	int	fdin = dup(tmpin);
//     int ret;
//     int fdout;
// 	(int) cmd_num;
// 	int	i;

// 	i = 0;
// 	while (i < cmd_num)
// 	{
// 		if (i != (cmd_num - 1))
// 		{
// 			int fdpipe[2];
// 			pipe(fdpipe);
// 			fdout = fdpipe[1];
// 			fdin = fdpipe[0];
// 		}
// 		dup2(fdout, 1);
// 		close(fdout);
// 		ret = fork();
// 		if (ret == 0)
// 		{
// 			if (simple_cmds[i]->redirects)
// 				handleRedirections(simple_cmds[i]->redirects);
// 			execvp(simple_cmds[i]->cmd[0], simple_cmds[i]->cmd);
// 			exit_minishell(NULL, EXIT_EXECVE);
// 		}
// 		i++;
// 	}
// 	dup2(tmpin, 0);
// 	dup2(tmpout, 1);
// 	close(tmpin);
// 	close(tmpout);
// 	waitpid(ret, NULL, 0);
// }