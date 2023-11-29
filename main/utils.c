/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:48:52 by vbartos           #+#    #+#             */
/*   Updated: 2023/11/29 11:04:46 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// strs_count
// - counts the amount of strings in a 2D array;
size_t	strs_count(char **strs)
{
	size_t	strs_count;

	strs_count = 0;
	while (strs[strs_count] != NULL)
		strs_count++;
	return (strs_count);
}

// free_array
// - frees a dynamically allocated 2D array;
int		free_array(char **strs)
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