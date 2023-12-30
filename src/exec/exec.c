/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 11:33:30 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/30 16:08:39 by vbartos          ###   ########.fr       */
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
		exec_multicmds(data, simple_cmds);
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
// 	int cmd_num_int = (int)cmd_num;
// 	int	i;
// 	char			*path;
// 	char			**env_copy;
// 	t_simple_cmds	*content;

// 	env_copy = exec_copyenv(data);
// 	i = 0;
// 	while (i < cmd_num_int)
// 	{
// 		content = (t_simple_cmds *) simple_cmds->content;
// 		dup2(fdin, STDIN);
// 		close(fdin);
// 		if (i == (cmd_num_int - 1))
// 		{
// 			fdout = dup(tmpout);
// 		}
// 		else 
// 		{
// 			int fdpipe[2];
// 			pipe(fdpipe);
// 			fdout = fdpipe[1];
// 			fdin = fdpipe[0];
// 		}
// 		// if (content->redirects)
// 		// 	exec_handleredirect(content->redirects);
// 		dup2(fdout, 1);
// 		close(fdout);
// 		if (exec_isbuiltin(content->cmd[0]))
// 			exec_runbuiltin(data, content->cmd);
// 		else
// 		{
// 			ret = fork();
// 			if (ret == -1)
// 			{
// 				free(env_copy);
// 				exit_current_prompt(NULL);
// 			}
// 			if (ret == 0)
// 			{
// 				path = exec_findpath(data, content->cmd[0]);
// 				if (path != NULL)
// 				{
// 					execve(path, content->cmd, env_copy);
// 					free(path);
// 				}
// 				free(env_copy);
// 				exit_current_prompt(NULL);
// 			}
// 		}
// 		i++;
// 		simple_cmds = simple_cmds->next;
// 	}
// 	dup2(tmpin, STDIN);
// 	dup2(tmpout, STDOUT);
// 	close(tmpin);
// 	close(tmpout);
// 	waitpid(ret, NULL, 0);
// }

void exec_multicmds(t_data *data, t_list *simple_cmds)
{
	int				saved_in;
	int				saved_out;
	int				pipefd[2];
	int				fd_in;
	int				fd_out;
	t_simple_cmds	*content;

	saved_in = dup(STDIN);
	saved_out = dup(STDOUT);
	fd_in = dup(saved_in);
	while (simple_cmds)
	{
		content = (t_simple_cmds *) simple_cmds->content;
		if (content->redirects)
			exec_handleredirect(content->redirects);
		if (simple_cmds->next == NULL)
		{
			if (content->redirects)
				exec_handleredirect(content->redirects);
			else
				fd_out = dup(saved_out);
		}
		else
		{
			// fprintf(stdout, "Here\n");
			pipe(pipefd);
			fd_in = pipefd[0];
			fd_out = pipefd[1];
		}
		dup2(fd_in, STDIN);
		close(fd_in);
		dup2(fd_out, STDOUT);
		close(fd_out);
		exec_cmd(data, content);
		simple_cmds = simple_cmds->next;
	}
	dup2(saved_in, STDIN);
	dup2(saved_out, STDOUT);
	close(saved_in);
	close(saved_out);
	wait(NULL);
}

void exec_cmd(t_data *data, t_simple_cmds *content)
{
	char	**env_copy;
	int		pid;
	char	*path;

	if (exec_isbuiltin(content->cmd[0]))
		exec_runbuiltin(data, content->cmd);
	else
	{
		env_copy = exec_copyenv(data);
		pid = fork();
		if (pid == -1)
		{
			free(env_copy);
			exit_current_prompt(NULL);
		}
		if (pid == 0)
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
		free(env_copy);
		free(path);
	}
}