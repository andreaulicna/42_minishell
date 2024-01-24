/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 15:31:17 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/24 15:33:30 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief	Counts single and double quotes in a string.
 * 
 * This function keeps track of the number of single and double quotes
 * encountered in the input string. It's used in the word-counting process
 * to correctly handle quoted substrings as part of a word.
 * 
 * @param	c			character to check
 * @param	s_quotes	pointer to the count of single quotes
 * @param	d_quotes	pointer to the count of double quotes
 */

void	count_qoutes(char c, unsigned int *s_quotes, unsigned int *d_quotes)
{
	if (c == '\'' && !(*d_quotes % 2))
		*s_quotes += 1;
	else if (c == '"' && !(*s_quotes % 2))
		*d_quotes += 1;
}

/**
 * @brief	Checks if both single and double quotes are paired.
 * 
 * This function determines whether both single and double quotes are paired in
 * a balanced manner. It's crucial for identifying if a word is enclosed in
 * a pair of matched quotes or not.
 * 
 * @param	s_quotes	count of single quotes
 * @param	d_quotes	count of double quotes
 * @return	int			1 if single and double quotes are paired, otherwise 0
 */

int	quotes_pair(unsigned int s_quotes, unsigned int d_quotes)
{
	if (!(s_quotes % 2) && !(d_quotes % 2))
		return (1);
	return (0);
}
