/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 06:30:01 by vbartos           #+#    #+#             */
/*   Updated: 2024/02/03 21:39:52 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief Updates the OLDPWD and PWD environment variables in the linked list.
 *
 * This function takes the current working directory (cwd) and the previous
 * working directory (oldpwd) as arguments. It updates the OLDPWD and PWD
 * environment variables in the linked list with the provided values.
 *
 * @param oldpwd The previous working directory.
 * @param data   A pointer to the structure containing environment variables.
 */
static void	ft_cd_update(char *oldpwd, t_data *data)
{
	t_list	*oldpwd_env;
	t_list	*pwd_env;
	t_env	*content;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (oldpwd == NULL || cwd == NULL)
		return ;
	oldpwd_env = env_find(data->env_list, "OLDPWD");
	pwd_env = env_find(data->env_list, "PWD");
	if (oldpwd_env != NULL)
	{
		content = (t_env *) oldpwd_env->content;
		free(content->value);
		content->value = ft_strdup(oldpwd);
	}
	if (pwd_env != NULL)
	{
		content = (t_env *) pwd_env->content;
		free(content->value);
		content->value = ft_strdup(cwd);
	}
	free(cwd);
}

/**
 * @brief Returns a pointer to the desired path stored in the environment
 * variables list.
 *
 * Given the name of an environment variable (path_name), this function searches
 * for it in the linked list of environment variables and returns a pointer
 * to its value.
 *
 * @param path_name The name of the environment variable to retrieve.
 * @param data      A pointer to the data structure containing environment
 * variables.
 * @return A pointer to the value of the specified environment variable, or
 * NULL if not found.
 */
static char	*ft_cd_getpath(char *path_name, t_data *data)
{
	t_list	*path_env;
	t_env	*content;

	path_env = env_find(data->env_list, path_name);
	if (path_env == NULL)
		return (NULL);
	content = (t_env *) path_env->content;
	return (content->value);
}

/**
 * @brief Handles the case when there are no arguments to 'cd' (aka HOME).
 *
 * This function changes the current working directory to the HOME directory,
 * updates the OLDPWD and PWD environment variables, and sets the exit status
 * accordingly.
 *
 * @param oldpwd The previous working directory.
 * @param data   A pointer to the data structure containing environment
 * variables.
 */
static void	ft_cd_home(char *oldpwd, t_data *data)
{
	char	*path;
	int		ret;

	path = ft_cd_getpath("HOME", data);
	if (path == NULL)
		ret = 1;
	else
		ret = chdir(path) != 0;
	if (ret != 0)
		ft_putendl_fd("minishell: cd: HOME not set", STDERR);
	else
		ft_cd_update(oldpwd, data);
	data->exit_status = ret;
}

/**
 * @brief Handles the case of the '-' (minus) argument to 'cd' (aka OLDPWD).
 *
 * This function changes the current working directory to the previous working
 * directory (as stored in OLDPWD), updates the OLDPWD and PWD environment
 * variables,
 * and sets the exit status accordingly.
 *
 * @param oldpwd The previous working directory.
 * @param data   A pointer to the data structure containing environment
 * variables.
 */
static void	ft_cd_previous(char *oldpwd, t_data *data)
{
	char	*path;
	int		ret;

	path = ft_cd_getpath("OLDPWD", data);
	if (path == NULL)
		ret = 1;
	else
		ret = chdir(path) != 0;
	if (ret != 0)
		ft_putendl_fd("minishell: cd: OLDPWD not set", STDERR);
	else
		ft_cd_update(oldpwd, data);
	data->exit_status = ret;
}

/**
 * @brief Changes the current working directory.
 *
 * This function implements the 'cd' command, which changes the current
 * working directory. It handles different cases, such as no arguments,
 * the presence of a tilde (~), or the use of the '-' (minus) argument.
 * It updates the OLDPWD and PWD environment variables accordingly and sets
 * the exit status based on the success of the 'chdir' operation.
 *
 * @param args An array of command arguments.
 * @param data A pointer to the data structure containing environment variables.
 */
void	ft_cd(char **args, t_data *data)
{
	int		ret;
	char	cwd[PATH_MAX];

	if (strs_count(args) > 2)
	{
		ft_cd_toomanyargs(data);
		return ;
	}
	getcwd(cwd, PATH_MAX);
	if (args[1] == NULL || (args[1][0] == '~' && !args[1][1]))
	{
		ft_cd_home(cwd, data);
		return ;
	}
	else if (args[1] && !ft_strncmp(args[1], "-", 1))
	{
		ft_cd_previous(cwd, data);
		return ;
	}
	ret = chdir(args[1]) != 0;
	if (ret != 0)
		ft_cd_nosuchfile(args[1]);
	else
		ft_cd_update(cwd, data);
	data->exit_status = ret;
}
