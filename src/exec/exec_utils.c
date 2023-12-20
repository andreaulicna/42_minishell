/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 07:49:03 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/20 16:57:09 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

// exec_findpath
// - searches for the path of an executable command;
char	*exec_findpath(t_data *data, char *cmd)
{
	t_list	*path_env;
	t_env	*content;
	char	**path_arr;
	char	*temp;
	char	*working_path;
	
	path_env = env_find(data->env_list, "PATH");
	content = (t_env *)path_env;
	path_arr = ft_split(ft_strdup(content->value), ':');
	while (path_arr != NULL)
	{
		temp = ft_strjoin(*path_arr, "/");
		working_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(working_path, F_OK) == 0)
		{
			free_array(path_arr);
			return (working_path);
		}
		free(working_path);
		path_arr++;
	}
	free_array(path_arr);
	ft_putstr_fd(cmd, STDERR);
	ft_putendl_fd(": command not found", STDERR);
	return (NULL);
}

// exec_copyenv
// - copies the env linked list into char array for the purposes of 'execve';
char	**exec_copyenv(t_data *data)
{
	char	**arr;
	t_list	*current;
	int		i;
	t_env	*content;

	arr = (char **)malloc(sizeof(char *) * (ft_lstsize(data->env_list) + 1));
	if (!arr)
		exit_minishell(NULL, EXIT_MALLOC);
	current = data->env_list;
	i = 0;
	while (i < ft_lstsize(data->env_list) && current != NULL)
	{
		content = (t_env *)current->content;
		arr[i] = content->full_string;
		i++;
		current = current->next;
	}
	arr[i] = NULL;
	return (arr);
}

// exec_isbuiltin
// - returns 0 if provided command is builtin;
int	exec_isbuiltin(char *cmd)
{
	if (ft_strncmp(cmd, "cd", 2) == 0
		|| ft_strncmp(cmd, "echo", 4) == 0
		|| ft_strncmp(cmd, "env", 3) == 0
		|| ft_strncmp(cmd, "exit", 4) == 0
		|| ft_strncmp(cmd, "export", 6) == 0
		|| ft_strncmp(cmd, "pwd", 3) == 0
		|| ft_strncmp(cmd, "unset", 5) == 0)
		return (1);
	else
		return (0);
}

// exec_runbuiltin
// - runs provided builtin;
void exec_runbuiltin(t_data *data, char **cmd)
{
	if (ft_strncmp(cmd[0], "cd", 2) == 0)
		ft_cd(cmd, data);
	else if (ft_strncmp(cmd[0], "echo", 4) == 0)
		ft_echo(cmd);
	else if (ft_strncmp(cmd[0], "env", 4) == 0)
		ft_env(data);
	else if (ft_strncmp(cmd[0], "exit", 4) == 0)
		ft_exit(cmd, data);
	else if (ft_strncmp(cmd[0], "export", 6) == 0)
		ft_export(cmd, data);
	else if (ft_strncmp(cmd[0], "pwd", 3) == 0)
		ft_pwd();
	else if (ft_strncmp(cmd[0], "unset", 5) == 0)
		ft_unset(cmd, data);
}

// exec_handleredirect
// - handles the redirection accordingly to specified symbol and file;
// - function is called from inside a child process;
void	exec_handleredirect(t_list *redirects)
{
	char	**redirect_arr;
	int		fd;

	redirect_arr = (char **)redirects->content;
	if (ft_strncmp(redirect_arr[0], ">", 1) == 0)
	{
		fd = open(redirect_arr[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		dup2(fd, STDOUT);
		close(fd);
	}
	else if (ft_strncmp(redirect_arr[0], ">>", 2) == 0)
	{
		fd = open(redirect_arr[1], O_WRONLY | O_CREAT | O_APPEND, 0666);
		dup2(fd, STDOUT);
		close(fd);
	}
	else if (ft_strncmp(redirect_arr[0], "<", 1) == 0)
	{
		fd = open(redirect_arr[1], O_RDONLY);
		dup2(fd, STDIN);
		close(fd);
	}
}