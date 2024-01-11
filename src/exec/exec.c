/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 11:33:30 by vbartos           #+#    #+#             */
/*   Updated: 2024/01/11 14:38:43 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

int exec(t_data *data, t_list *simple_cmds)
{
	t_simple_cmds	*content;
	int				cmds_num;

	orig_fds_save(&data->orig_fdin, &data->orig_fdout);
	content = (t_simple_cmds *) simple_cmds->content;
	cmds_num = ft_lstsize(simple_cmds);
	if (simple_cmds->next == NULL && is_builtin(content->cmd[0]))
	{
		if (content->redirects)
			handle_redirect(content->redirects, content->hd_file);
		run_builtin(data, content->cmd);
	}
	else
		exec_pipeline(data, simple_cmds, cmds_num);
	orig_fds_restore(data->orig_fdin, data->orig_fdout);
	return (0);
}

void exec_pipeline(t_data *data, t_list *simple_cmds, int cmds_num)
{
	int	fd_pipe[2];
	int	fd_input;
	int	fd_output;
	int	pid_list[cmds_num];
	int	i;

	fd_input = STDIN;
	i = 0;
	while(i < cmds_num)
	{
		if (simple_cmds->next != NULL)
			fd_output = pipe_create(fd_pipe);
		else
			fd_output = STDOUT;
		pid_list[i] = fork_cmd(data, simple_cmds, fd_input, fd_output);
		if (fd_input != STDIN)
			close(fd_input);
		if (simple_cmds->next != NULL)
			fd_input = pipe_close(fd_pipe);
		if (simple_cmds->next == NULL && ft_lstsize(simple_cmds) > 1)
			close(fd_input);
		simple_cmds = simple_cmds->next;
		i++;
	}
	wait_for_pipeline(data, pid_list, cmds_num);
}

int fork_cmd(t_data *data, t_list *simple_cmds, int fd_input, int fd_output)
{
	int				pid;
	t_simple_cmds	*content;
	
	content = (t_simple_cmds *) simple_cmds->content;
	pid = fork();
	if (pid == -1)
	{
		ft_putendl_fd("minishell: fork: Resource temporarily unavailable", 2);
		exit_current_prompt(NULL);
	}
	if (pid == 0)
	{
		// fprintf(stderr, "Child process created\n");
		pipe_redirect(fd_input, fd_output);
		if (content->redirects)
			handle_redirect(content->redirects, content->hd_file);
		if (is_builtin(content->cmd[0]))
		{
			run_builtin(data, content->cmd);
			data->exit_status = 0;
			exit(data->exit_status);
		}
		else
			run_exec(data, content);
	}
	return (pid);
}

void run_exec(t_data *data, t_simple_cmds *content)
{
	char	**env_cpy;
	char	*path;

	env_cpy = env_copy(data);
	if (access(content->cmd[0], F_OK) == 0)
		execve(content->cmd[0], content->cmd, env_cpy);
	path = find_exe_path(data, content->cmd[0]);
	if (path != NULL)
	{
		execve(path, content->cmd, env_cpy);
		free(path);
	}
	free(env_cpy);
	data->exit_status = 126;
	exit(data->exit_status);
}

void run_builtin(t_data *data, char **cmd)
{
	if (ft_strncmp(cmd[0], "cd", 2) == 0)
		ft_cd(cmd, data);
	else if (ft_strncmp(cmd[0], "echo", 4) == 0)
		ft_echo(cmd);
	else if (ft_strncmp(cmd[0], "env", 3) == 0)
		ft_env(data);
	else if (ft_strncmp(cmd[0], "exit", 4) == 0)
		ft_exit(cmd, data);
	else if (ft_strncmp(cmd[0], "export", 6) == 0)
		ft_export(cmd, data);
	else if (ft_strncmp(cmd[0], "pwd", 3) == 0)
		ft_pwd();
	else if (ft_strncmp(cmd[0], "unset", 5) == 0)
		ft_unset(cmd, data);
}