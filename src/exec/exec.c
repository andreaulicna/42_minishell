/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 11:33:30 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/20 17:36:42 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

// get a number of commands
// - for each command, create a child process
// - before that, check it command is builtin, not executable
int	exec(t_data *data, t_list *simple_cmds)
{
	size_t			cmd_num;
	t_simple_cmds	*content;

	content = (t_simple_cmds *) simple_cmds->content;
	cmd_num = ft_lstsize(simple_cmds);
	if (cmd_num == 1)
		if (exec_isbuiltin(content->cmd[0]))
			exec_runbuiltin(data, content->cmd);
		else
			exec_singlecmd(data, simple_cmds);
	else if (cmd_num > 1)
		// exec_multicmds(data, simple_cmds, cmd_num);
		return (1);
	return (0);
}

void exec_singlecmd(t_data *data, t_list *simple_cmds)
{
	pid_t	pid;
	t_simple_cmds	*content;
	// int		status;
	char	*path;

	pid = fork();
	if (pid == -1)
		perror("fork");
	content = (t_simple_cmds *) simple_cmds->content;
	if (pid == 0)
	{
		if (content->redirects)
			exec_handleredirect(content->redirects);
		path = exec_findpath(data, content->cmd[0]);
		if (path != NULL)
		{
			execve(path, content->cmd, exec_copyenv(data));
			free(path);
		}
		exit_minishell(NULL, EXIT_EXECVE);
	}
	else
		wait(NULL);
}

// void exec_multicmds(t_data *data, t_list *simple_cmds, size_t cmd_num)
// {
// 	int	tmpin = dup(0);
// 	int	tmpout = dup(1);
// 	int	fdin = dup(tmpin);
//     int ret;
//     int fdout;
// 	(int) cmd_num;

// 	for (int i = 0; i < cmd_num; i++)
// 	{
// 		dup2(fdin, 0);
// 		close(fdin);
// 		if (i == cmd_num - 1)
// 			fdout = dup(tmpout);
// 		else
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
// 	}
// 	dup2(tmpin, 0);
// 	dup2(tmpout, 1);
// 	close(tmpin);
// 	close(tmpout);
// 	waitpid(ret, NULL, 0);
// }