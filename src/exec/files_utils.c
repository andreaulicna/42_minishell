/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 10:21:50 by vbartos           #+#    #+#             */
/*   Updated: 2024/02/01 10:36:12 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * Displays an error message indicating that the user is attempting to execute
 * a directory.
 *
 * @param content The structure containing the command information.
 */
static void	is_directory(t_simple_cmds *content)
{
	ft_putstr_fd(content->cmd[0], STDERR);
	ft_putendl_fd(": Is a directory", STDERR);
	exit_minishell(NULL, 126);
}

/**
 * Displays an error message indicating that the user does not posses required
 * executable rights and exits the child process.
 *
 * @param content The structure containing the command information.
 */
static void	permission_denied(t_simple_cmds *content)
{
	ft_putstr_fd(content->cmd[0], STDERR);
	ft_putendl_fd(": Permission denied", STDERR);
	exit_minishell(NULL, 126);
}

/**
 * Displays an error message indicating that the specified file or directory
 * does not exist and exits the child process.
 *
 * @param content The structure containing the command information.
 */
static void	no_such_file(t_simple_cmds *content)
{
	ft_putstr_fd(content->cmd[0], STDERR);
	ft_putendl_fd(": No such file or directory", STDERR);
	exit_minishell(NULL, 127);
}

/**
 * Checks if the specified command is a file or directory and performs
 * necessary actions, like permission checks. Runs command if it is
 * an absolute path to an executable.
 * 
 * @param content The structure containing the command and arguments.
 * @param env_cpy The copy of the environment variables.
 */
void	check_for_files(t_simple_cmds *content, char **env_cpy)
{
	int			i;
	struct stat	path_stat;

	i = 0;
	while (content->cmd[0][i] == ' ')
		i++;
	if (content->cmd[0][i] == '/' ||
		(content->cmd[0][i] == '.' && content->cmd[0][i + 1] == '/'))
	{
		if (access(content->cmd[0], F_OK) == 0)
		{
			stat(content->cmd[0], &path_stat);
			if (S_ISDIR(path_stat.st_mode))
				is_directory(content);
			else if (access(content->cmd[0], X_OK) == -1)
				permission_denied(content);
			execve(content->cmd[0], content->cmd, env_cpy);
		}
		else
			no_such_file(content);
	}
}