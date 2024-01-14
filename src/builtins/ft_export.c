/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 07:08:30 by vbartos           #+#    #+#             */
/*   Updated: 2024/01/14 17:59:13 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * Formats and prints an environment variable in the format
 * "declare -x VARNAME="VALUE""
 *
 * @param env_var The environment variable to format and print
 */
void	ft_export_format(char *env_var)
{
	int		equalsign_pos;
	char	*substr1;
	char	*substr2;

	ft_putstr_fd("declare -x ", STDOUT);
	if (ft_strchr(env_var, '=') != NULL)
	{
		equalsign_pos = ft_strlen(env_var) - ft_strlen(ft_strchr(env_var, '='));
		substr1 = ft_substr(env_var, 0, equalsign_pos);
		substr2 = ft_substr(env_var, equalsign_pos + 1,
				ft_strlen(env_var) - ft_strlen(substr1));
		ft_putstr_fd(substr1, STDOUT);
		ft_putstr_fd("=\"", STDOUT);
		ft_putstr_fd(substr2, STDOUT);
		ft_putstr_fd("\"", STDOUT);
		free(substr1);
		free(substr2);
	}
	else
		ft_putstr_fd(env_var, STDOUT);
	ft_putstr_fd("\n", 1);
}

/**
 * Sorts an array of environment variables in ascending order based on
 * their length and lexicographical order.
 * 
 * @param env_arr The array of environment variables to be sorted.
 */
void	ft_export_sort(char **env_arr)
{
	long unsigned int	i;
	long unsigned int	j;
	size_t				bigger_len;
	char				*temp;

	i = 0;
	while (i < (strs_count(env_arr) - 2))
	{
		j = 0;
		while (j < (strs_count(env_arr) - 2 - i))
		{
			if (ft_strlen(env_arr[j]) > ft_strlen(env_arr[j + 1]))
				bigger_len = ft_strlen(env_arr[j]);
			else
				bigger_len = ft_strlen(env_arr[j + 1]);
			if (ft_strncmp(env_arr[j], env_arr[j + 1], bigger_len) > 0)
			{
				temp = env_arr[j];
				env_arr[j] = env_arr[j + 1];
				env_arr[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

/**
 * @brief Export the environment variables as an array of strings.
 * 
 * This function takes a pointer to a data structure containing the
 * environment variables and exports them as an array of strings.
 * The exported array is sorted and formatted before being printed.
 * 
 * @param data A pointer to the data structure containing the environment
 * variables.
 */
void	ft_export_list(t_data *data)
{
	char	**arr;
	t_list	*current;
	int		i;
	t_env	*env;

	arr = (char **)malloc(sizeof(char *) * (ft_lstsize(data->env_list) + 1));
	if (!arr)
		exit_minishell(NULL, EXIT_MALLOC);
	current = data->env_list;
	i = 0;
	while (i < ft_lstsize(data->env_list) && current != NULL)
	{
		env = (t_env *)current->content;
		arr[i] = env->full_string;
		i++;
		current = current->next;
	}
	arr[i] = NULL;
	ft_export_sort(arr);
	i = -1;
	while (++i < ft_lstsize(data->env_list))
		ft_export_format(arr[i]);
	free(arr);
}

/**
 * Adds a new environment variable to the environment list.
 * If the variable already exists, it updates its value.
 * 
 * @param args The array of arguments containing the variable to be
 * added/updated.
 * @param data The data structure containing the environment list.
 * @param i The index of the argument in the args array.
 */
void ft_export_add(char **args, t_data *data, int i)
{
	t_list	*possible_duplicate;
	size_t	equal_pos;
	char	*var_name;

	equal_pos = 0;
	while (args[i][equal_pos] != '=' && args[i][equal_pos] != '\0')
		equal_pos++;
	var_name = ft_substr(args[i], 0, equal_pos);
	possible_duplicate = env_find(data->env_list, var_name);
	if (possible_duplicate == NULL)
		env_add(&data->env_list, args[i]);
	else
	{
		if (args[i][equal_pos] == '=')
		{
			env_remove(&data->env_list, var_name);
			env_add(&data->env_list, args[i]);
		}
	}
	free(var_name);
}

/**
 * @brief Export environment variables or display the current environment
 * variables.
 *
 * This function is responsible for exporting environment variables to the
 * environment variables list or displaying the current environment variables.
 * If no arguments are provided, it displays the current environment variables.
 * If arguments are provided, it validates each argument and adds it to the
 * environment variables if it is valid.
 * If an argument is not a valid identifier, it displays an error message and
 * returns an error status.
 *
 * @param args The arguments passed to the ft_export command.
 * @param data The data structure containing the environment variables.
 * @return Returns 0 on success, 1 on failure.
 */
int	ft_export(char **args, t_data *data)
{
	int		i;

	if (strs_count(args) == 1)
	{
		ft_export_list(data);
		return (0);
	}
	i = 1;
	while (args[i] != NULL)
	{
		if (ft_export_validate(args[i]) == 0)
			ft_export_add(args, data, i);
		else
		{
			ft_putstr_fd("minishell: export: ", STDERR);
			ft_putstr_fd(args[i], STDERR);
			ft_putendl_fd(": not a valid identifier", STDERR);
			data->exit_status = 1;
			return (1);
		}
		i++;
	}
	return (0);
}