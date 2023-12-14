/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:48:52 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/13 18:58:35 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

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
