/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_checkers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 15:23:41 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/29 10:42:30 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief	Checks for the presence and closure of quotes in the input string.
 * 
 * It prints an error message to STDERR if unclosed quotes are found.
 * 
 * @param	input	input string to be checked for quotes
 * @return	int		returns 1 if all quotes are closed and balanced; 0 otherwise
 */
int	check_quotes(char *input)
{
	int				i;
	unsigned int	quotes;

	quotes = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' && quotes == 0)
			quotes = 1;
		else if (input[i] == '\'' && quotes == 1)
			quotes = 0;
		else if (input[i] == '"' && quotes == 0)
			quotes = 2;
		else if (input[i] == '"' && quotes == 2)
			quotes = 0;
		i++;
	}
	if (quotes != 0)
	{
		ft_putstr_fd("Minishell cannot interpret unclosed quotes, ", 2);
		ft_putstr_fd("please close them.\n", 2);
		return (0);
	}
	return (1);
}

int	check_input_null(char *input)
{
	if (input == NULL)
		return (0);
	else
		add_history(input);
	return (1);
}

int	check_enter_space(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (!(7 <= input[i] && input[i] <= 13)
			&& input[i] != 32 && input[i] != '\0')
			return (1);
		i++;
	}
	return (0);
}
