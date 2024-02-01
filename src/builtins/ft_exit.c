/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 14:26:16 by vbartos           #+#    #+#             */
/*   Updated: 2024/01/24 09:13:39 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief Helper function for ft_exit; checks if the string has non-numeric
 * characters.
 *
 * This function is a helper for the ft_exit function. It checks if the
 * provided string contains non-numeric characters, allowing for an optional
 * leading '+' or '-' sign.
 *
 * @param str The string to be checked for non-numeric characters.
 * @return 1 if the string contains non-numeric characters, 0 otherwise.
 */
int	ft_exit_checknum(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief Exits the minishell with an optional exit status.
 *
 * This function implements the 'exit' command in the minishell. It allows
 * the user to exit the shell with an optional exit status. If there are too
 * many arguments, a non-numeric argument is provided, or the exit status is
 * invalid, an error message is printed to STDERR, and the exit status is set
 * accordingly.
 *
 * @param args An array of strings representing the arguments for the 'exit'
 * command.
 * @param data A pointer to the data structure for handling shell-related
 * information.
 */
void	ft_exit(char **args, t_data *data)
{
	int	exit_status;
	int	arg_num;

	arg_num = 0;
	while (args[arg_num] != NULL)
		arg_num++;
	exit_status = 0;
	ft_putendl_fd("exit", STDOUT);
	if (arg_num > 2)
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR);
		data->exit_status = 1;
		return ;
	}
	else if (arg_num == 2 && ft_exit_checknum(args[1]) == 1)
	{
		ft_putstr_fd("minishell: exit: ", STDERR);
		ft_putstr_fd(args[1], STDERR);
		ft_putendl_fd(": numeric argument required", STDERR);
		exit_status = 2;
	}
	else if (arg_num == 2)
		exit_status = ft_atoi(args[1]) % 256;
	exit_minishell(data, exit_status);
}
