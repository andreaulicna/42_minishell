/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 16:26:04 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/12 10:29:02 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// ft_unset
// - removes a variable from the environment variable linked list;

int	ft_unset(char **args, t_data *data)
{
	int	i;
	
	if (strs_count(args) == 1)
		return (0);
	i = 1;
	while (args[i] != NULL)
	{
		env_remove(&data->env_list, args[i]);
		i++;
	}
	return (0);
}