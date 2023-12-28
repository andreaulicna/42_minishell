/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 11:33:30 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/28 10:56:40 by vbartos          ###   ########.fr       */
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
		exec_multicmds(data, simple_cmds, cmd_num);
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

void exec_multicmds(t_data *data, t_list *simple_cmds, size_t cmd_num)
{
	int	tmpin = dup(STDIN);
	int	tmpout = dup(STDOUT);
	int	fdin = dup(tmpin);
    int ret;
    int fdout;
	int cmd_num_int = (int)cmd_num;
	int	i;
	char			*path;
	char			**env_copy;
	t_simple_cmds	*content;

	env_copy = exec_copyenv(data);
	i = 0;
	printf("number of cmds: %d\n", cmd_num_int);
	while (i < cmd_num_int)
	{
		dup2(fdin, STDIN);
		close(fdin);
		if (i == (cmd_num_int - 1))
		{
			fdout = dup(tmpout);
		}
		else 
		{
			printf("pipes created\n");
			int fdpipe[2];
			pipe(fdpipe);
			fdout = fdpipe[1];
			fdin = fdpipe[0];
		}
		dup2(fdout, 1);
		close(fdout);
		ret = fork();
		if (ret == -1)
		{
			free(env_copy);
			exit_current_prompt(NULL);
		}
		if (ret == 0)
		{
			printf("child process created\n");
			content = (t_simple_cmds *) simple_cmds->content;
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
		i++;
		simple_cmds = simple_cmds->next;
	}
	dup2(tmpin, STDIN);
	dup2(tmpout, STDOUT);
	close(tmpin);
	close(tmpout);
	waitpid(ret, NULL, 0);
}