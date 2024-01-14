/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:24:31 by vbartos           #+#    #+#             */
/*   Updated: 2024/01/14 18:31:14 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

// ft_env
// - prints the list of environment variables;
// - only prints variables with assigned values;
void ft_env(t_data *data)
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
