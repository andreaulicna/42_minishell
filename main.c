/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:33:13 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/26 15:33:59 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"
#include "libftprintf/ft_printf.h"

char	*get_hostname(void)
{
	int	fd;
	char	*content;
	char	*hostname;
	int	i;


	fd = open("/proc/sys/kernel/hostname", O_RDONLY);
	content = get_next_line(fd);
	close(fd);
	i = 0;
	while (content[i] && content[i] != '.')
		i++;
	hostname = (char *) malloc(sizeof(char) * (i + 3));
	i = 0;
	while (content[i] && content[i] != '.')
	{
		hostname[i] = content[i];
		i++;
	}
	hostname[i] = ':';
	hostname[i + 1] = '\0';
	free(content);
	return(hostname);
}

char	*get_username(char *env[])
{
	int	i;
	char	*username;

	i = 0;
	while (ft_strncmp("USER=", env[i], 5))
		i++;
	username = ft_strjoin(env[i] + 5, "@");
	return (username);
}

char	*get_directory(char *env[])
{
	int	i;
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
	directory_final = ft_strjoin(directory, "$");
	free(directory);
	return (directory_final);
}

char	*construct_prompt(char *username, char *hostname, char *directory)
{
	char	*tmp_prompt;
	char	*prompt;

	tmp_prompt = ft_strjoin(username, hostname);
	prompt = ft_strjoin(tmp_prompt, directory);
	free(tmp_prompt);
	return (prompt);
}

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

int main(int argc, char **argv, char *env[])
{
	(void) argc;
	(void) argv;
	char *prompt;
	char	*input;

	while(1)
	{
		prompt = set_prompt(env);
		input = readline(prompt);
		if (input == NULL)
		{
			free (prompt);
			free (input);
			printf("exit\n");
			exit (0);
		}
		free (prompt);
	}
	free (input);
	return (0);
}