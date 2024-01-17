/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 15:20:49 by vbartos           #+#    #+#             */
/*   Updated: 2024/01/17 11:59:04 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

// exit_minishell
// - upon first call, initializes static pointer to data struct;
// - static data pointer allows for easier free calls;
// - frees all allocated memory;
// - exits with passed error;
void	exit_minishell(t_data *data, int exit_status)
{
	static t_data	*static_data;

	if (!static_data)
		static_data = data;
	else
	{
		free_data(static_data);
		exit(exit_status);
	}
}

void	exit_current_prompt(t_data *data)
{
	static t_data	*static_data;

	if (!static_data)
		static_data = data;
	else
	{
		free_data_current_prompt(static_data);
		minishell_loop(static_data);
	}
}
