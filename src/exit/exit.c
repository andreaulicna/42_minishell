/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 15:20:49 by vbartos           #+#    #+#             */
/*   Updated: 2024/01/14 19:03:55 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief Exits the minishell program with the specified exit number.
 *
 * Upon first call, initializes static pointer to data struct, which allows
 * for easier free() calls. Frees the allocated memory before calling the exit()
 * function.
 *
 * @param data The pointer to the data structure.
 * @param exit_num The exit number to be used.
 */
void	exit_minishell(t_data *data, int exit_num)
{
	static t_data	*static_data;
	int				exit_status;

	if (!static_data)
		static_data = data;
	else
	{
		static_data->exit_status = exit_num;
		exit_status = static_data->exit_status;
		free_data(static_data);
		exit(exit_status);
	}
}

/**
 * @brief Exits the current prompt and frees the associated data.
 * 
 * Upon first call, initializes static pointer to data struct, which allows
 * for easier free() calls. Frees the data associated with it and then calls
 * the `minishell_loop` function to start a new prompt with the updated
 * `static_data`.
 * 
 * @param data Pointer to the `t_data` structure.
 */
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
