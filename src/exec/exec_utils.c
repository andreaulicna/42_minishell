/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 07:49:03 by vbartos           #+#    #+#             */
/*   Updated: 2024/01/22 13:31:11 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * Finds the executable path for a given command by searching through the PATH
 * environment variable.
 * 
 * @param data The data structure containing the environment list.
 * @param cmd The command to find the executable path for.
 * @return The executable path if found, NULL otherwise.
 */
char	*find_exe_path(t_data *data, char *cmd)
{
	t_list	*path_env;
	t_env	*content;
	char	**path_arr;
	char	*temp;
	char	*working_path;
	int		i;

	path_env = env_find(data->env_list, "PATH");
	content = (t_env *) (path_env->content);
	path_arr = ft_split(content->value, ':');
	i = 0;
	while (path_arr[i] != NULL)
	{
		temp = ft_strjoin(path_arr[i], "/");
		working_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(working_path, F_OK) == 0)
		{
			free_array(path_arr);
			return (working_path);
		}
		free(working_path);
		i++;
	}
	free_array(path_arr);
	return (NULL);
}

/**
 * Creates a copy of the environment list as an array of strings.
 * 
 * @param data The data structure containing the environment list.
 * @return The copied environment list as an array of strings.
 */
char	**env_copy(t_data *data)
{
	char	**arr;
	t_list	*current;
	int		i;
	t_env	*content;

	arr = (char **)malloc(sizeof(char *) * (ft_lstsize(data->env_list) + 1));
	if (!arr)
		exit_minishell(NULL, EXIT_MALLOC);
	current = data->env_list;
	i = 0;
	while (i < ft_lstsize(data->env_list) && current != NULL)
	{
		content = (t_env *)current->content;
		arr[i] = content->full_string;
		i++;
		current = current->next;
	}
	arr[i] = NULL;
	return (arr);
}

/**
 * Checks if a command is a built-in command.
 * 
 * @param cmd The command to check.
 * @return 1 if the command is a built-in command, 0 otherwise.
 */
int is_builtin(char *cmd)
{
	if ((ft_strncmp(cmd, "cd", 2) == 0 && ft_strlen(cmd) == 2)
		|| (ft_strncmp(cmd, "echo", 4) == 0 && ft_strlen(cmd) == 4)
		|| (ft_strncmp(cmd, "env", 3) == 0 && ft_strlen(cmd) == 3)
		|| (ft_strncmp(cmd, "exit", 4) == 0 && ft_strlen(cmd) == 4)
		|| (ft_strncmp(cmd, "export", 6) == 0 && ft_strlen(cmd) == 6)
		|| (ft_strncmp(cmd, "pwd", 3) == 0 && ft_strlen(cmd) == 3)
		|| (ft_strncmp(cmd, "unset", 5) == 0 && ft_strlen(cmd) == 5))
		return (1);
	else
		return (0);
}

/**
 * @brief	Waits for all processes in the pipeline to finish.
 * 
 * The if and else statement ensures there is no fd left open
 * in the parent process for one command and piped commands, respectively.
 *
 * @param	data		pointer to the t_data structure (for exit_status)
 * @param	cmds_num	number of commands in the pipeline
 * @param	fd_pipe		2d array of file descriptors
 * @param	i			current position in fd_pipe
 */
void	wait_for_pipeline(t_data *data, int cmds_num, int **fd_pipe, int i)
{
	int	j;
	int	status;

	j = 0;
	while (j < cmds_num)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status))
			data->exit_status = WEXITSTATUS(status);
		j++;
	}
	if (cmds_num == 1)
		close(fd_pipe[i - 1][PIPE_READ]);
	else
		close(fd_pipe[i - 2][PIPE_WRITE]);
}
