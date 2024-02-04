/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 12:09:39 by aulicna           #+#    #+#             */
/*   Updated: 2024/02/04 14:27:45 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief	Retrieves the hostname of the system from 
 * "/proc/sys/kernel/hostname" and formats it as "[hostname]:". The ":" is 
 * added at the end of the hostname to make it easier to construct the whole
 * prompt in contruct_prompt(), especially with regards to using ft_strjoin()
 * repeatedly.
 *
 * @return	char*	hostname formatted as "[hostname]:"
 */

static char	*get_hostname(void)
{
	int		fd;
	char	*content;
	char	*hostname;
	int		i;

	fd = open("/proc/sys/kernel/hostname", O_RDONLY);
	content = get_next_line(fd);
	close(fd);
	i = 0;
	while (content[i] && content[i] != '.')
		i++;
	hostname = (char *) malloc(sizeof(char) * (i + 3));
	if (!hostname)
		exit_minishell(NULL, EXIT_MALLOC);
	i = 0;
	while (content[i] && content[i] != '.' && content[i] != '\n')
	{
		hostname[i] = content[i];
		i++;
	}
	hostname[i] = ':';
	hostname[i + 1] = '\0';
	free(content);
	return (hostname);
}

/**
 * @brief	Extracts the current working directory from the environment
 * variables "PWD" and "HOME".
 * 
 * The default to get PWD is to use the env_find function. However, when that
 * returns NULL (meaning that the variable has been unset in the current
 * session, which is our minishell), we fall back on the getcwd function that
 * gets the system current working directory. We cannot just use getcwd because
 * then we'd be ignoring value of PWD set via export.
 * 
 * The default to get HOME is env_find too. However, when that returns NULL,
 * directory is simply set to the cwd value. And since there is no HOME to
 * assess for potentially including ~, the code falls into return.
 *   
 * If env_find returns HOME, the function checks if the directory is within
 * the home directory. If so, it formats the directory as 
 * "~[cwd without home directory preamble that is replaced with ~]>$".
 * Otherwise it formats it as "[cwd in full patf format]$". 
 * 
 * The "$" is added at the end of the directory to make it easier to construct
 * the whole prompt in contruct_prompt(), especially with regards to using
 * ft_strjoin() repeatedly.
 *
 * @param	env 	array of environment variables
 * @return	char*	directory formatted either as ~[cwd without home directory
 * preamble that is replaced with ~]>$" or "[cwd in full path format]$
 */

static char	*get_directory(t_list *env_list)
{
	char	*cwd;
	char	*home;
	char	*directory;
	t_list	*cwd_env;
	t_list	*home_env;

	cwd_env = env_find(env_list, "PWD");
	if (cwd_env == NULL)
		cwd = getcwd(NULL, 0);
	else
		cwd = ft_strdup(((t_env *) cwd_env->content)->value);
	home_env = env_find(env_list, "HOME");
	if (home_env == NULL)
		directory = ft_strdup(cwd);
	else
	{
		home = ((t_env *) home_env->content)->value;
		if (ft_strnstr(cwd, home, ft_strlen(cwd)) != NULL)
			directory = ft_strjoin("~", cwd + ft_strlen(home));
		else
			directory = ft_strdup(cwd);
	}
	free(cwd);
	return (directory);
}

/**
 * @brief 	Constructs the final prompt by concatenating the username, hostname,
 * and directory strings.
 *
 * @param	username	username formatted as "[username]@"
 * @param	hostname	hostname formatted as "[hostname]:"
 * @param	directory	directory formatted either as ~[cwd without home
 * directory preamble that is replaced with ~]>$" or "[cwd in full path format]$
 * @return	char*		final prompt formatted either as
 * "[username]@[hostname]:~[cwd without home directory preamble that is replaced
 * with ~]>$" or "[username]@[hostname]:[cwd in full path]>$"
 */

static char	*construct_prompt(char *username, char *hostname, char *directory)
{
	char	*tmp_prompt;
	char	*prompt;

	tmp_prompt = ft_strjoin(username, hostname);
	prompt = ft_strjoin(tmp_prompt, directory);
	free(tmp_prompt);
	return (prompt);
}

/**
 * @brief	Sets up the complete prompt by calling various functions to retrieve
 * the username, hostname, and directory, and assembling them into the final
 * prompt.
 *
 * @param	env 	array of environment variables
 * @return	char*		final prompt formatted either as
 * "[username]@[hostname]:~[cwd without home directory preamble that is replaced
 * with ~]>$" or "[username]@[hostname]:[cwd in full path]>$"
 */

char	*set_prompt(t_list *env_list)
{
	char	*hostname;
	char	*prompt;
	char	*username;
	char	*directory_tmp;
	char	*directory;

	username = ft_strjoin(getenv("USER"), "@");
	hostname = get_hostname();
	directory_tmp = get_directory(env_list);
	directory = ft_strjoin(directory_tmp, "$ ");
	free(directory_tmp);
	prompt = construct_prompt(username, hostname, directory);
	free(username);
	free(hostname);
	free(directory);
	return (prompt);
}
