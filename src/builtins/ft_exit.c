/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 14:26:16 by vbartos           #+#    #+#             */
/*   Updated: 2024/01/19 06:41:04 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

// ft_exit_checknum
// - helper function for ft_exit;
// - checks if char argument has non-numeric characters;
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

// ft_exit
// - if 'too many arguments', bash doesn't exit, in all other cases it does;
// - if one non-numeric arg provided, exit_status defaults to 255;
// - otherwise exits with provided exit_status (0 if not provided);
void ft_exit(char **args, t_data *data)
{
	int	exit_status;
	int	arg_num;

	arg_num = 0;
	while(args[arg_num] != NULL)
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
