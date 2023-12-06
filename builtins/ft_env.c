/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:24:31 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/06 14:09:40 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// ft_env
// - prints the list of environment variables;
int	ft_env(t_data *data)
{
	t_list	*current;
	t_env	*env;

	current = data->env_list;
	while (current != NULL)
	{
		env = (t_env *)current->content;
		ft_putstr_fd(env->name, STDOUT);
		ft_putchar_fd('=', STDOUT);
		ft_putendl_fd(env->value, STDOUT);
		current = current->next;
	}
	return (0);
}