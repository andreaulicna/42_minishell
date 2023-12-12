/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 12:09:39 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/12 12:38:15 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

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
 * @brief	Extracts the username from the environment variable "USER" and
 * formats it as "username@" using the env_find function. 
 * 
 * The "@" is added at the end of the username to make it easier to construct
 * the whole prompt in contruct_prompt(), especially with regards to using
 * ft_strjoin() repeatedly.
 *
 * @param	env 	array of environment variables
 * @return	char*	username formatted as "[username]@"
 */

static char	*get_username(t_list *env_list)
{
	t_list	*username_env;
	t_env	*username_content;
	char	*username;

	username_env = env_find(env_list, "USER");
	username_content = (t_env *) username_env->content;
	username = ft_strjoin(username_content->value, "@");
	return (username);
}

/**
 * @brief	Extracts the current working directory from the environment
 * variable "PWD". Checks if the directory is within the home directory.
 * If so, formats the directory as "~[cwd without home directory preamble that
 * is replaced with ~]>$", otherwise it formats it as 
 * "[cwd in full patf format]$". The "$" is added at the end of the directory
 * to make it easier to construct the whole prompt in contruct_prompt(),
 * especially with regards to using ft_strjoin() repeatedly.
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
	cwd = ((t_env *) cwd_env->content)->value;
	home_env = env_find(env_list, "HOME");
	home = ((t_env *) home_env->content)->value;
	if (ft_strnstr(cwd, home, ft_strlen(cwd)) != NULL)
		directory = ft_strjoin("~", cwd + ft_strlen(home));
	else
		directory = ft_strdup(cwd);
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

	username = get_username(env_list);
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
