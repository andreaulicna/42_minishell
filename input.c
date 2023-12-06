/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 12:41:41 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/06 11:06:41 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"

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
