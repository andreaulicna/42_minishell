/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 07:08:30 by vbartos           #+#    #+#             */
/*   Updated: 2023/11/29 11:15:53 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// - if export with no args
// 	- print a list of env variables with string before ALPHABETICALLY;
// 	- to print alphabetically, copy the contents into char array, then sort;
// - if args
// 	- capital letters added into the existing list, lowercase to the end;
// 	- 

void	ft_export_format(char *env_var)
{
	int		i;
	int		flag_split;
	char	**split_strs;

	i = 0;
	flag_split = 0;
	split_strs = NULL;
	while (env_var[i] != '\0')
	{
		if (env_var[i] == '=')
			flag_split = 1;
		i++;
	}
	ft_putstr_fd("declare -x ", 1);
	if (flag_split == 1)
	{
		split_strs = ft_split(env_var, '=');
		ft_putstr_fd(split_strs[0], 1);
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(split_strs[1], 1);
		ft_putstr_fd("\"", 1);
	}
	else
	{
		ft_putstr_fd(env_var, 1);
	}
	ft_putstr_fd("\n", 1);
	// if (split_strs)
	// 	free_array(split_strs);
}

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

void	ft_export_list(t_data *data)
{
	char	**env_arr;
	t_list	*current;
	int		i;

	env_arr = (char **)malloc(sizeof(char *) * (ft_lstsize(data->env_list)));
	if (!env_arr)
		perror("ft_export array malloc");
	current = data->env_list;
	i = 0;
	while (i < ft_lstsize(data->env_list) && current != NULL)
	{
		env_arr[i] = current->content;
		i++;
		current = current->next;
	}
	ft_export_sort(env_arr);
	i = 0;
	while (i < ft_lstsize(data->env_list))
	{
		ft_export_format(env_arr[i]);
		i++;
	}
	free(env_arr);
}

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
		env_add(data->env_list, args[i]);
		i++;
	}
	return (0);
}