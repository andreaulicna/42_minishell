/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_shell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:55:35 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/22 13:09:48 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"
#include "libftprintf/ft_printf.h"

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	char	*input;
	char	**arr;
	char	*cmd;
	
	input = readline("simple-shell\% ");
	if (input == NULL)
	{
		printf("exit");
		return (-1);
	}
	arr = ft_split(input, ' ');
	if (arr != NULL)
	{
		cmd = arr[0];
		if (execve(cmd, arr, NULL) == -1)
		// no free here bcs execve replaces the current process
			perror("Error:");
	}
	free(input);
	free_arr(arr);
	return(0);
}