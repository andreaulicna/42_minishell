/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:48:52 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/05 17:03:55 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// init_data
// - initializes data struct;
int		init_data(t_data *data, char **envp)
{
	env_init(envp, data);
	//parser_init;
	//lexer_init;
	return (0);
}

// exit_minishell
// - frees all allocated memory;
// - exits with passed error;
void	exit_minishell(t_data *data, int exit_status)
{
	free_data(data);
	exit(exit_status);
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