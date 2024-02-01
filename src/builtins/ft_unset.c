/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 16:26:04 by vbartos           #+#    #+#             */
/*   Updated: 2024/02/01 11:21:05 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief Removes a variable from the environment variable linked list.
 *
 * This function removes one or more variables from the linked list of
 * environment variables. If no variables are provided, the function exits
 * with a status of 0. Otherwise, it iterates through the provided variable
 * names and removes them from the environment variable list.
 *
 * @param args An array of strings representing the names of variables to be
 * removed.
 * @param data A pointer to the data structure containing the linked list of
 * environment variables.
 */

void	ft_unset(char **args, t_data *data)
{
	int	i;

	if (strs_count(args) == 1)
	{
		data->exit_status = 0;
		return ;
	}
	i = 1;
	while (args[i] != NULL)
	{
		env_remove(&data->env_list, args[i]);
		i++;
	}
	data->exit_status = 0;
}
