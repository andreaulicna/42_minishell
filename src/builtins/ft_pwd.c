/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:02:24 by vbartos           #+#    #+#             */
/*   Updated: 2024/02/01 11:19:31 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief Prints the current working directory.
 *
 * This function uses the getcwd function to retrieve the current working
 * directory and prints it to the standard output. If successful, the directory
 * is printed; otherwise, the exit status is set to 1.
 *
 * @param data A pointer to the data structure for handling shell-related
 * information.
 */
void	ft_pwd(t_data *data)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_putendl_fd(cwd, STDOUT);
	else
		data->exit_status = 1;
	data->exit_status = 0;
}
