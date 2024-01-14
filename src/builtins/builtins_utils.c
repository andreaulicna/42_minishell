/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 17:52:32 by vbartos           #+#    #+#             */
/*   Updated: 2024/01/14 17:55:25 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * Validates the variable name for the ft_export command. Variable name cannot
 * start with a digit, be missing (nothing before '='), or contain a hyphen.
 * 
 * @param arg The argument to be validated.
 * @return 1 if the argument is invalid, 0 otherwise.
 */
int ft_export_validate(char *arg)
{
	int	i;

	if (ft_isdigit(arg[0]) || arg[0] == '=')
		return (1);
	i = 0;
	while (arg[i] != '\0' && arg[i] != '=')
	{
		if (arg[i] == '-')
			return (1);
		i++;
	}
	return (0);
}