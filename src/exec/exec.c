/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 11:33:30 by vbartos           #+#    #+#             */
/*   Updated: 2024/01/08 13:08:46 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * Executes a series of simple commands.
 * This function takes a pointer to a data structure and a linked list of simple
 * commands as input. It executes each simple command in the list, connecting
 * their input and output as necessary.
 *
 * @param data The data structure containing relevant information for execution.
 * @param simple_cmds The linked list of simple commands to be executed.
 * @return 0 on success.
 */
int	exec(t_data *data, t_list *simple_cmds)
{
	int	fd_pipe[2];
	int	fd_input;
	int	fd_output;

	fd_input = STDIN;
	orig_fds_save(&data->orig_fdin, &data->orig_fdout);
	while(simple_cmds != NULL)
	{
		if (simple_cmds->next != NULL)
			fd_output = pipe_create(fd_pipe);
		else
			fd_output = STDOUT;
		run_cmd(data, simple_cmds, fd_input, fd_output);
		if (fd_input != STDIN)
			close(fd_input);
		if (simple_cmds->next != NULL)
			fd_input = pipe_close(fd_pipe);
		if (simple_cmds->next == NULL && ft_lstsize(simple_cmds) > 1)
			close(fd_pipe[PIPE_READ]);
		simple_cmds = simple_cmds->next;
	}
	orig_fds_restore(data->orig_fdin, data->orig_fdout);
	return (0);
}

/**
 * Executes a command by either running a built-in command or executing
 * an external program.
 * 
 * @param data The data structure containing the shell's state and settings.
 * @param simple_cmds The list of simple commands to be executed.
 * @param fd_input The file descriptor for input redirection.
 * @param fd_output The file descriptor for output redirection.
 */
void run_cmd(t_data *data, t_list *simple_cmds, int fd_input, int fd_output)
{
	t_simple_cmds	*content;

	content = (t_simple_cmds *) simple_cmds->content;
	if (content->redirects)
			handle_redirect(content->redirects);
	if (is_builtin(content->cmd[0]))
		run_builtin(data, content->cmd, fd_input, fd_output);
	else
		run_exec(data, simple_cmds, fd_input, fd_output);
}

/**
 * Executes a command by forking a child process and running the command in it.
 * 
 * @param data The data structure containing the environment variables and other
 * information.
 * @param cmd The command to be executed.
 * @param fd_input The file descriptor for input redirection.
 * @param fd_output The file descriptor for output redirection.
 */
void run_exec(t_data *data, t_list *cmd, int fd_input, int fd_output)
{
	char			**env_cpy;
	int				pid;
	t_simple_cmds	*content;
	char			*path;
	
	pid = fork();
	if (pid == -1)
	{
		ft_putendl_fd("minishell: fork: Resource temporarily unavailable", 2);
		exit_current_prompt(NULL);
	}
	content = (t_simple_cmds *) cmd->content;
	if (pid == 0)
	{
		pipe_redirect(fd_input, fd_output);
		env_cpy = env_copy(data);
		path = find_exe_path(data, content->cmd[0]);
		if (path != NULL)
		{
			execve(path, content->cmd, env_cpy);
			free(path);
		}
		free(env_cpy);
		exit_current_prompt(NULL);
	}
	waitpid(pid, NULL, 0);
}

/**
 * Executes the built-in commands based on the given command.
 *
 * @param data The data structure containing the shell's state.
 * @param cmd The command to be executed.
 * @param fd_input The file descriptor for input redirection.
 * @param fd_output The file descriptor for output redirection.
 */
void run_builtin(t_data *data, char **cmd, int fd_input, int fd_output)
{
	pipe_redirect(fd_input, fd_output);
	if (ft_strncmp(cmd[0], "cd", 2) == 0)
		ft_cd(cmd, data);
	else if (ft_strncmp(cmd[0], "echo", 4) == 0)
		ft_echo(cmd);
	else if (ft_strncmp(cmd[0], "env", 4) == 0)
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