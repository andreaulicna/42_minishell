/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 16:26:04 by vbartos           #+#    #+#             */
/*   Updated: 2024/01/14 18:42:23 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

// ft_unset
// - removes a variable from the environment variable linked list;

void ft_unset(char **args, t_data *data)
{
	int	i;

	if (strs_count(args) == 1)
	{
		data->exit_status = 0;
		return;
	}
	i = 1;
	while (args[i] != NULL)
	{
		env_remove(&data->env_list, args[i]);
		i++;
	}
	data->exit_status = 0;
}
