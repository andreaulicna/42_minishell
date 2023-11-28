/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 06:30:01 by vbartos           #+#    #+#             */
/*   Updated: 2023/11/28 06:57:38 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_cd(char **args)
{
	if (chdir(args[1]) != 0)
	{
		//return_error (or exit_error?)
		perror("getcwd");
		return (1);
	}
	ft_pwd();
	return (0);
}