/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 07:08:30 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/12 10:34:21 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// ft_export_format
// - formats and prints all strings in 2D array akin to the export bash builtin;
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

// ft_export_sort
// - bubble sorts the 2D array into alphabetical order;
void	ft_export_sort(char **env_arr)
{
	size_t				arr_len;
	long unsigned int	i;
	long unsigned int 	j;
	size_t				bigger_len;
	char				*temp;

	arr_len = strs_count(env_arr) - 1;
	i = 0;
	while (i < (arr_len - 1))
	{
		j = 0;
		while (j < (arr_len - 1 - i))
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

// ft_export_list
// - mallocs the env linked list into a 2D array for sorting purposes;
// - sorts the array alphabetically via ft_export_sort;
// - formats and prints the entire array via ft_export_format;
// - frees the 2D array (not the strings themselves,
// 		as they are part of the linked list);
void	ft_export_list(t_data *data)
{
	char	**env_arr;
	t_list	*current;
	int		i;
	t_env	*env;

	env_arr = (char **)malloc(sizeof(char *)
				* (ft_lstsize(data->env_list) + 1));
	if (!env_arr)
	{
		perror("ft_export array malloc");
		return;
	}
	current = data->env_list;
	i = 0;
	while (i < ft_lstsize(data->env_list) && current != NULL)
	{
		env = (t_env *)current->content;
		env_arr[i] = env->full_string;
		i++;
		current = current->next;
	}
	env_arr[i] = NULL;
	ft_export_sort(env_arr);
	i = 0;
	while (i < ft_lstsize(data->env_list))
	{
		ft_export_format(env_arr[i]);
		i++;
	}
	free(env_arr);
}

// ft_export
// - if no args, prints a list of env variables ALPHABETICALLY;
// - if args, add varible into the env linked list;
int		ft_export(char **args, t_data *data)
{
	int	i;
	
	if (strs_count(args) == 1)
	{
		ft_export_list(data);
		return (0);
	}
	i = 1;
	while (args[i] != NULL)
	{
		env_add(&data->env_list, args[i]);
		i++;
	}
	return (0);
}