/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_shell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:55:35 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/22 17:00:25 by aulicna          ###   ########.fr       */
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

/**
 * @brief	This function extracts the PATH environment variable from the given 
 * environment variables and returns it.
 * 
 * @param	env		environment (global) variables that can be accessed by 
 * 					a program to obtain information about system's environment,
 * 					user, and configuration
*/

char	*get_path(char **env)
{
	int	i;

	i = 0;
	while (ft_strncmp("PATH=", env[i], 5))
		i++;
	return (env[i] + 5);
}

/**
 * @brief	This function  uses a list of directories (paths) and a command (cmd)
 * to constructs the full path to the command. It does so by searching for it in 
 * the specified directories. If the command is found, the full path is returned.
 * Otherwise, it returns NULL.
*/

static char	*get_command_full(char **paths, char *cmd)
{
	int		i;
	char	*cmd_full;
	char	*cmd_tmp;

	i = 0;
	while (paths[i])
	{
		cmd_tmp = ft_strjoin(paths[i], "/");
		cmd_full = ft_strjoin(cmd_tmp, cmd);
		free(cmd_tmp);
		if (access(cmd_full, 0) == 0)
			return (cmd_full);
		free(cmd_full);
		i++;
	}
	return (NULL);
}

void	command_not_found(char **input_arr, char **paths, char *cmd_full)
{
	ft_putstr_fd("Command not found: ", 2);
	ft_putstr_fd(input_arr[0], 2);
	ft_putstr_fd("\n", 2);
	free_arr(paths);
	free_arr(input_arr);
	free(cmd_full);
	exit(1);
}

int	main(int argc, char **argv, char *env[])
{
	(void) argc;
	(void) argv;
	char	*input;
	char	**input_arr;
	char	*cmd;
	char	*env_path;
	char	**paths;
	char	*cmd_full;
	
	input = readline("simple-shell\% ");
	if (input == NULL)
	{
		printf("exit");
		return (-1);
	}
	input_arr = ft_split(input, ' ');
	free(input);
	if (input_arr != NULL)
	{
		cmd = input_arr[0];
		env_path = get_path(env);
		paths = ft_split(env_path, ':');
		cmd_full = get_command_full(paths, cmd);
		if (cmd_full != NULL && execve(cmd_full, input_arr, env) == -1)
			command_not_found(input_arr, paths, cmd_full);
	}
	free_arr(input_arr);
	free_arr(paths);
	free(cmd_full);
	return(0);
}