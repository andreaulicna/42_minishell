/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 17:52:32 by vbartos           #+#    #+#             */
/*   Updated: 2024/02/01 11:23:42 by vbartos          ###   ########.fr       */
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
int	ft_export_validate(char *arg)
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

/**
 * Displays an error message when the specified file or directory does not
 * exist.
 *
 * @param arg The name of the file or directory that does not exist.
 */
void	ft_cd_nosuchfile(char *arg)
{
	ft_putstr_fd("minishell: cd: ", STDERR);
	ft_putstr_fd(arg, STDERR);
	ft_putendl_fd(": No such file or directory", STDERR);
}

/**
 * Displays an error message indicating that too many arguments were provided
 * for the "cd" command. Sets the exit status of the program to 1.
 *
 * @param data: A pointer to the data structure containing information
 * about the shell.
 */
void	ft_cd_toomanyargs(t_data *data)
{
	ft_putendl_fd("minishell: cd: too many arguments", STDERR);
	data->exit_status = 1;
}
