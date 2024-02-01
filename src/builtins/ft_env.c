/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:24:31 by vbartos           #+#    #+#             */
/*   Updated: 2024/02/01 11:19:42 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief Prints the list of environment variables with assigned values.
 *
 * This function prints the list of environment variables along with their
 * assigned values to the standard output. It iterates through the linked list
 * of environment variables, checks for variables with non-NULL values,
 * and prints them in the format "name=value".
 *
 * @param data A pointer to the data structure containing the linked list
 * of environment variables.
 */
void	ft_env(t_data *data)
{
	t_list	*current;
	t_env	*env;

	current = data->env_list;
	while (current != NULL)
	{
		env = (t_env *)current->content;
		if (env->value != NULL)
		{
			ft_putstr_fd(env->name, STDOUT);
			ft_putchar_fd('=', STDOUT);
			ft_putendl_fd(env->value, STDOUT);
		}
		current = current->next;
	}
	data->exit_status = 0;
}
