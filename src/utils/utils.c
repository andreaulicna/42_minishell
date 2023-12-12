/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:48:52 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/12 15:21:24 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

// init_data
// - initializes data struct;
// - first call of exit_minishell initializes static pointer to data struct;
int	init_data(t_data *data, char **envp)
{
	env_init(envp, data);
	exit_minishell(data, 0);
	return (0);
}

// strs_count
// - counts the amount of strings in a 2D array;
size_t	strs_count(char **strs)
{
	size_t	strs_count;

	strs_count = 0;
	while (strs[strs_count])
		strs_count++;
	return (strs_count);
}
