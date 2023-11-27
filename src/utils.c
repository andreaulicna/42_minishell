/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:48:52 by vbartos           #+#    #+#             */
/*   Updated: 2023/11/27 12:23:06 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// count the amount of strings in a 2D array;
size_t	strs_count(char **strs)
{
	size_t	strs_count;

	strs_count = 0;
	while (strs[strs_count] != NULL)
		strs_count++;
	return (strs_count);
}

// free a 2D array;
int		free_list(char **strs)
{
	int	i;

	i = 0;
	while (strs[i] != NULL)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
	return (0);
}