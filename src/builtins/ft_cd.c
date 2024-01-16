/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 06:30:01 by vbartos           #+#    #+#             */
/*   Updated: 2024/01/16 18:00:10 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

// ft_cd_update
// - updates the OLDPWD and PWD env variables in the linked list;
void	ft_cd_update(char *oldpwd, t_data *data)
{
	t_list	*oldpwd_env;
	t_list	*pwd_env;
	t_env	*content;
	char	cwd[PATH_MAX];

	getcwd(cwd, PATH_MAX);
	if (oldpwd == NULL || cwd == NULL)
		return ;
	oldpwd_env = env_find(data->env_list, "OLDPWD");
	pwd_env = env_find(data->env_list, "PWD");
	content = (t_env *) oldpwd_env->content;
	free(content->value);
	content->value = ft_strdup(oldpwd);
	content = (t_env *) pwd_env->content;
	free(content->value);
	content->value = ft_strdup(cwd);
}

// ft_cd_getpath
// - returns a pointer to a desired path stored in the env variables list;
char	*ft_cd_getpath(char *path_name, t_data *data)
{
	t_list	*path_env;
	t_env	*content;

	path_env = env_find(data->env_list, path_name);
	if (path_env == NULL)
		return (NULL);
	content = (t_env *) path_env->content;
	return (content->value);
}

// ft_cd_home
// - handles the case when there are no arguments to 'cd' (aka HOME);
int	ft_cd_home(char *oldpwd, t_data *data)
{
	int	ret;

	ret = chdir(ft_cd_getpath("HOME", data));
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

	ret = chdir(ft_cd_getpath("OLDPWD", data)) != 0;
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
		ft_putendl_fd("minishell: cd: too many arguments", STDERR);
		return (1);
	}
	getcwd(cwd, PATH_MAX);
	if (args[1] == NULL || (args[1][0] == '~' && !args[1][1]))
		return (ft_cd_home(cwd, data));
	else if (args[1] && !ft_strncmp(args[1], "-", 1))
		return (ft_cd_previous(cwd, data));
	ret = chdir(args[1]);
	if (ret != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR);
		ft_putstr_fd(args[1], STDERR);
		ft_putendl_fd(": No such file or directory", STDERR);
	}
	else
		ft_cd_update(cwd, data);
	return (ret);
}
