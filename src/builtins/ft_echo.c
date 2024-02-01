/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 13:45:57 by vbartos           #+#    #+#             */
/*   Updated: 2024/02/01 11:19:49 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief Simulates the bash builtin 'echo'.
 *
 * This function simulates the behavior of the 'echo' command in Bash. It prints
 * the specified arguments to the standard output, separated by spaces. It also
 * checks for the -n flag, which suppresses the trailing newline character.
 *
 * @param args An array of strings representing the args for the 'echo' command.
 * @param data A pointer to the data structure for handling shell-related info.
 */
void	ft_echo(char **args, t_data *data)
{
	int	n_flag;
	int	i;

	n_flag = 0;
	i = 1;
	if (strs_count(args) > 1)
	{
		if (args[1] && ft_strncmp(args[1], "-n", 2) == 0)
		{
			n_flag = 1;
			i++;
		}
		while (args[i] != NULL)
		{
			ft_putstr_fd(args[i], STDOUT_FILENO);
			if (args[i + 1])
				ft_putstr_fd(" ", STDOUT_FILENO);
			i++;
		}
	}
	if (n_flag == 0)
		ft_putstr_fd("\n", STDOUT_FILENO);
	data->exit_status = 0;
}
