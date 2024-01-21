/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 11:33:30 by vbartos           #+#    #+#             */
/*   Updated: 2024/01/21 17:41:36 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * Executes a list of simple commands.
 * 
 * Saves the STDIN and STDOUT file descriptors for later restoration.
 * If there is only one command and it's a builtin, runs it in the main process.
 * For all other cases, runs a pipeline with child processes.
 * 
 * @param data The data structure containing the shell's state and settings.
 * @param simple_cmds The linked list of simple commands to execute.
 */
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
			handle_redirect(data, content->redirects, content->hd_file);
		run_builtin(data, content->cmd);
	}
	else
		exec_pipeline(data, simple_cmds, cmds_num);
	orig_fds_restore(data->orig_fdin, data->orig_fdout);
	return (0);
}

/**
 * Executes a pipeline of simple commands.
 *
 * This function takes a data structure, a linked list of simple commands,
 * and the number of commands in the pipeline. It creates pipes for
 * inter-process communication and forks child processes to execute each
 * command in the pipeline. The input and output file descriptors are managed
 * accordingly to redirect the input and output of each command.
 * The function waits for all child processes to complete before returning.
 *
 * @param data The data structure containing information about the shell environment.
 * @param simple_cmds The linked list of simple commands in the pipeline.
 * @param cmds_num The number of commands in the pipeline.
 */
void exec_pipeline(t_data *data, t_list *simple_cmds, int cmds_num)
{
	int	**fd_pipe;
	int	pid_list[cmds_num];
	int	i;
	int close_val;

	i = 0;
	fd_pipe = malloc(sizeof(int *) * (ft_lstsize(simple_cmds)));
	while(i < cmds_num)
	{
		fd_pipe[i] = malloc(sizeof(int) * 2);
	//	if (simple_cmds->next != NULL)
	//	{
		if (pipe(fd_pipe[i]) == -1)
		{
			ft_putendl_fd("minishell: pipe: Too many open files", 2);
			exit_current_prompt(NULL);
		}
	//	fd_output = new_pipe[i][PIPE_WRITE];
	//	test_write = fd_output;
	//	}
	//	else
	//	if (simple_cmds->next == NULL)
		pid_list[i] = fork_cmd(data, simple_cmds, fd_pipe, i);
//		if (fd_input != STDIN)
//		{
		close_val = close(fd_pipe[i][PIPE_WRITE]);
		if (i > 0)
			close_val = close(fd_pipe[i - 1][PIPE_READ]);
//		}
//		if (simple_cmds->next != NULL)
//		{
//			close_val = close(new_pipe[i][PIPE_WRITE]);
//			fd_input = new_pipe[i][PIPE_READ];
//			test_read = new_pipe[i][PIPE_READ];
//		}
//			//fd_input = pipe_close(new_pipe[i]);
//		if (simple_cmds->next == NULL && list_size > 1)
//		{
//			close_val = close(fd_input);
//		}
		simple_cmds = simple_cmds->next;
		i++;
	}
	wait_for_pipeline(data, pid_list, cmds_num);
//	if: solves 4 open fds at the end of the program if only one command
//	else: solves 4 open fds at the end of the program if pipe
	if (ft_lstsize(data->simple_cmds) == 1)
		close(fd_pipe[i - 1][PIPE_READ]);
	else
		close_val = close(fd_pipe[i - 2][PIPE_WRITE]);
	free_pipe(fd_pipe, ft_lstsize(data->simple_cmds));
	(void) close_val;
}

/**
 * Forks a new process to execute a command.
 *
 * @param data The data structure containing the shell's state.
 * @param simple_cmds The list of simple commands to execute.
 * @param fd_input The file descriptor for input redirection.
 * @param fd_output The file descriptor for output redirection.
 * @return The process ID of the child process.
 */
int fork_cmd(t_data *data, t_list *simple_cmds, int **fd_pipe, int i)
{
	int				pid;
	t_simple_cmds	*content;

	content = (t_simple_cmds *)simple_cmds->content;
	pid = fork();
	if (pid == -1)
	{
		ft_putendl_fd("minishell: fork: Resource temporarily unavailable", 2);
		exit_current_prompt(NULL);
	}
	if (pid == 0)
	{
	//	pipe_redirect(fd_input, fd_output);
		close(fd_pipe[i][PIPE_READ]);
		if (i > 0)
			dup2(fd_pipe[i - 1][PIPE_READ], STDIN_FILENO);
		if (simple_cmds->next != NULL)
			dup2(fd_pipe[i][PIPE_WRITE], STDOUT_FILENO);
		// solves 4 open fds in child
		close(fd_pipe[i][PIPE_WRITE]);
		if (content->redirects)
			handle_redirect(data, content->redirects, content->hd_file);
		// solves still reachable in child
		free(fd_pipe[i]);
		free(fd_pipe);
		if (is_builtin(content->cmd[0]))
		{
			run_builtin(data, content->cmd);
			exit_minishell(NULL, 0);
		}
		else
			run_exec(data, content);
	}
	return (pid);
}

/**
 * Executes the given command. First checks if the command was given in and
 * absolute path. If so, executes it. If not, parses the command name and
 * attempts to generate a path for the executable.
 * 
 * @param data The data structure containing the shell's state.
 * @param content The structure containing the parsed command.
 */
void run_exec(t_data *data, t_simple_cmds *content)
{
	char    **env_cpy;
	char    *path;

	env_cpy = env_copy(data);
	if (access(content->cmd[0], F_OK) == 0)
		execve(content->cmd[0], content->cmd, env_cpy);
	path = find_exe_path(data, content->cmd[0]);
	if (path != NULL)
	{
		execve(path, content->cmd, env_cpy);
		free(path);
	}
	else if (path == NULL)
	{
		free(env_cpy);
		ft_putstr_fd(content->cmd[0], STDERR);
		ft_putendl_fd(": command not found", STDERR);
		exit_minishell(NULL, 127);
	}
	free(env_cpy);
	exit_minishell(NULL, 126);
}

/**
 * Executes the appropriate built-in command based on the given command.
 *
 * @param data The data structure containing the shell's state.
 * @param cmd The command to be executed.
 */
void run_builtin(t_data *data, char **cmd)
{
	if (ft_strncmp(cmd[0], "cd", 2) == 0)
		ft_cd(cmd, data);
	else if (ft_strncmp(cmd[0], "echo", 4) == 0)
		ft_echo(cmd, data);
	else if (ft_strncmp(cmd[0], "env", 3) == 0)
		ft_env(data);
	else if (ft_strncmp(cmd[0], "exit", 4) == 0)
		ft_exit(cmd, data);
	else if (ft_strncmp(cmd[0], "export", 6) == 0)
		ft_export(cmd, data);
	else if (ft_strncmp(cmd[0], "pwd", 3) == 0)
		ft_pwd(data);
	else if (ft_strncmp(cmd[0], "unset", 5) == 0)
		ft_unset(cmd, data);
}