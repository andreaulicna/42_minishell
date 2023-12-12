/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 06:30:01 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/12 10:57:51 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

// ft_cd_update
// - updates the OLDPWD and PWD env variables in the linked list;
void	ft_cd_update(char *oldpwd, t_data *data)
{
	t_list	*oldpwd_env;
	t_list	*pwd_env;
	char	cwd[PATH_MAX];

	getcwd(cwd, PATH_MAX);
	if (oldpwd == NULL || cwd == NULL)
		return ;
	oldpwd_env = env_find(data->env_list, "OLDPWD");
	pwd_env = env_find(data->env_list, "PWD");
	free(oldpwd_env->content);
	oldpwd_env->content = ft_strjoin("OLDPWD=", oldpwd);
	free(pwd_env->content);
	pwd_env->content = ft_strjoin("PWD=", cwd);
}

// ft_cd_getpath
// - returns a pointer to a desired path stored in the env variables list;
// - + len skips the variable key, + 1 skips the '=' sign;
char	*ft_cd_getpath(char *path_name, size_t len, t_data *data)
{
	t_list	*path_env;
	char	*path;

	path_env = env_find(data->env_list, path_name);
	if (path_env == NULL)
		return (NULL);
	path = (char *)path_env->content;
	return (path + len + 1);
}

// ft_cd_home
// - handles the case when there are no arguments to 'cd' (aka HOME);
int	ft_cd_home(char *oldpwd, t_data *data)
{
	int	ret;

	ret = chdir(ft_cd_getpath("HOME", 4, data));
	if (ret != 0)
		ft_putendl_fd("minishell: cd: HOME not set", STDERR);
	else
		ft_cd_update(oldpwd, data);
	return (ret);
}

// ft_cd_previous
// - handles the case of '-' (minus) argument to 'cd' (aka OLDPWD);
int	ft_cd_previous(char *oldpwd, t_data *data)
{
	int	ret;

	ret = chdir(ft_cd_getpath("OLDPWD", 6, data)) != 0;
	if (ret != 0)
		ft_putendl_fd("minishell: cd: OLDPWD not set", STDERR);
	else
		ft_cd_update(oldpwd, data);
	return (ret);
}

// ft_cd
// - changes the current working directory;
// - if more than two arguments, HOME or OLDPWD not set, return error;
// - if path found, change current working directory and update PWD, OLDPWD;
int	ft_cd(char **args, t_data *data)
{
	int		ret;
	char	cwd[PATH_MAX];

	if (strs_count(args) > 2)
	{
		perror("too many arguments");
		return (1);
	}
	getcwd(cwd, PATH_MAX);
	if (args[1] == NULL)
		ret = ft_cd_home(cwd, data);
	else if (args[1] && ft_strncmp(args[1], "-", 1))
		ret = ft_cd_previous(cwd, data);
	else
	{
		ret = chdir(args[1]);
		if (ret != 0)
		{
			ft_putstr_fd("minishell: cd: ", STDERR);
			ft_putstr_fd(args[1], STDERR);
			ft_putendl_fd(": No such file or directory", STDERR);
		}
		else
			ft_cd_update(cwd, data);
	}
	return (ret);
}
