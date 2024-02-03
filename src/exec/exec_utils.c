/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 07:49:03 by vbartos           #+#    #+#             */
/*   Updated: 2024/02/02 15:41:30 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief Converts the "PATH" environment variable into an array of strings.
 * 
 * This function takes a pointer to a `t_data` structure and retrieves the value
 * of the "PATH" environment variable. It then splits the value into individual
 * paths using the ':' delimiter and stores them in a dynamically allocated
 * array of strings. The resulting array is returned.
 * 
 * @param data A pointer to the `t_data` structure.
 * @return A dynamically allocated array of strings containing the individual
 * paths from the "PATH" environment variable.
 */
static char	**paths_into_arr(t_data *data)
{
	t_list	*path_env;
	t_env	*content;
	char	**path_arr;

	path_env = env_find(data->env_list, "PATH");
	content = (t_env *)(path_env->content);
	path_arr = ft_split(content->value, ':');
	return (path_arr);
}

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
	char	**path_arr;
	char	*temp;
	char	*working_path;
	int		i;

	if (!ft_strncmp(cmd, ".", 2) || !ft_strncmp(cmd, "..", 3))
		return (NULL);
	path_arr = paths_into_arr(data);
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
int	is_builtin(char *cmd)
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
 * @brief	Prints an error message when a fork creation fails.
*/
void	fork_process(int *pid)
{
	*pid = fork();
	if (*pid == -1)
	{
		ft_putendl_fd("minishell: fork: Resource temporarily unavailable", 2);
		exit_current_prompt(NULL);
	}
}
