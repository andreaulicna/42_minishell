/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_checkers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 14:44:13 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/24 14:49:12 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief 	Checks if a string contains a dollar sign.
 * 
 * @param	str	input string to be checked
 * @return	int	returns 1 if the string contains a dollar sign; 0 otherwise
 */
int	contains_dollar(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief	Checks the context of the dollar sign in a string.
 * 
 * This function analyzes the context of a dollar sign ('$') in a string 'str'
 * at position 'j'. It examines nearby characters to determine the context
 * of the dollar sign within the string.
 * 
 * @param	str	input string to check
 * @param	j	index of the dollar sign in the string
 * @return	int	returns a flag representing the context:
 *              5:	$ is enclosed in single quotes
 * 				4:	$ is followed by \ and enclosed in double quotes
 * 				3:	$ is followed or preceded with \ (not in double quotes)
 *              2:	$ is followed by a space, a quote (' or ")
 * 					or there is nothing else following it
 *              1:	$ is followed by a question mark (?)
 *              0: 	$ appears in a context that can be expanded
 */
int	checker_dollar(char *str, int j)
{
	if ((str[0] == '\'' && str[ft_strlen_custom(str) - 1] == '\''))
		return (5);
	if (str[0] == '"' && str[ft_strlen_custom(str) - 1] == '"'
		&& str[j + 1] == '\\')
		return (4);
	if (j - 1 >= 0)
	{
		if (str[j - 1] == '\\')
			return (3);
	}
	if (str[j + 1] == '\\')
		return (3);
	if (!str[j + 1] || str[j + 1] == ' ' || str[j + 1] == '\''
		|| str[j + 1] == '"')
		return (2);
	if (str[j + 1] == '?')
		return (1);
	return (0);
}
