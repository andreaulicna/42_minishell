/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 12:41:41 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/27 12:58:31 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"

int	check_quotes(char *input)
{
	int	i;
	unsigned int	s_quotes;
	unsigned int	d_quotes;

	while (input[i])
	{
		if (input[i] == '\'')
			s_quotes = 1;
		else if (input[i] == '\'' && s_quotes == 1)
			s_quotes = 0;
		else if (input[i] == '"')
			d_quotes = 1;
		else if (input[i] == '"' && d_quotes == 1)
			d_quotes = 0;
		i++;
	}
	if (s_quotes != 0 || d_quotes != 0)
	{
		ft_putstr_fd("Minishell cannot interpret unclosed quotes, ", 2);
//		ft_putstr_fd("please close them.", 2);
		return (0);
	}
	return (1);
}