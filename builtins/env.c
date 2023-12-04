/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:24:31 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/04 09:58:16 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// ft_env
// - gets the environment variable via main function argument, prints it;
int	ft_env(t_data *data)
{
	t_list *current;

	current = data->env_list;
	while (current != NULL)
	{
		ft_printf("%s\n", current->content);
		current = current->next;
	}
	return (0);
}