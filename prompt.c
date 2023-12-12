/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 12:09:39 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/26 17:01:45 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"

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
 * formats it as "username@". The "@" is added at the end of the username
 * to make it easier to construct the whole prompt in contruct_prompt(),
 * especially with regards to using ft_strjoin() repeatedly.
 *
 * @param	env 	array of environment variables
 * @return	char*	username formatted as "[username]@"
 */

static char	*get_username(char *env[])
{
	int		i;
	char	*username;

	i = 0;
	while (ft_strncmp("USER=", env[i], 5))
		i++;
	username = ft_strjoin(env[i] + 5, "@");
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

static char	*get_directory(char *env[])
{
	int		i;
	char	*cwd;
	char	*home;
	char	*directory;
	char	*directory_final;

	i = 0;
	while (ft_strncmp("PWD=", env[i], 4))
		i++;
	cwd = env[i] + 4;
	i = 0;
	while (ft_strncmp("HOME=", env[i], 5))
		i++;
	home = env[i] + 5;
	if (ft_strnstr(cwd, home, ft_strlen(cwd)) != NULL)
		directory = ft_strjoin("~", cwd + ft_strlen(home));
	else
		directory = ft_strdup(cwd);
	directory_final = ft_strjoin(directory, "$ ");
	free(directory);
	return (directory_final);
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

char	*set_prompt(char *env[])
{
	char	*hostname;
	char	*prompt;
	char	*username;
	char	*directory;

	username = get_username(env);
	hostname = get_hostname();
	directory = get_directory(env);
	prompt = construct_prompt(username, hostname, directory);
	free(username);
	free(hostname);
	free(directory);
	return (prompt);
}
