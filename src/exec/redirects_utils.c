/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 07:49:03 by vbartos           #+#    #+#             */
/*   Updated: 2024/02/01 15:49:50 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"


/**
 * @brief Determines the output mode for redirection based on the specified
 * filename and redirection type.
 *
 * @param filename The name of the file to be used for redirection.
 * @param redirection An integer indicating the type of redirection (GREATER,
 * GREATER_2, or other).
 * @return The file descriptor (fd) for the output mode.
 */
static int	determine_output_mode(char *filename, int redirection)
{
	int	fd;

	if (redirection == GREATER)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else if (redirection == GREATER_2)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
	else
		fd = 0;
	return (fd);
}

/**
 * @brief Handles output redirection by checking file existence and permissions,
 *        then redirecting standard output if necessary.
 *
 * @param filename The name of the file for output redirection.
 * @param fd The file descriptor for the output mode.
 * @return Returns 0 on success, 1 on failure.
 */
static int	handle_output(char *filename, int fd)
{
	if (access(filename, F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(filename, STDERR);
		ft_putendl_fd(": No such file or directory", STDERR);
		return (1);
	}
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(filename, STDERR);
		ft_putendl_fd(": Permission denied", STDERR);
		return (1);
	}
	if (fd > 0 && dup2(fd, STDOUT) < 0)
	{
		ft_putendl_fd("minishell: pipe error", STDERR);
		close(fd);
		return (1);
	}
	if (fd > 0)
		close(fd);
	return (0);
}

/**
 * @brief Handles input redirection by opening the specified file in
 * read-only mode, checking for permissions, and redirecting standard input.
 *
 * @param filename The name of the file for input redirection.
 * @return Returns 0 on success, 1 on failure.
 */
static int	handle_input(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(filename, STDERR);
		ft_putendl_fd(": No such file or directory", STDERR);
		return (1);
	}
	if (fd > 0 && dup2(fd, STDIN) < 0)
	{
		ft_putendl_fd("minishell: pipe error", STDERR);
		close(fd);
		return (1);
	}
	if (access(filename, R_OK) == -1)
	{
		ft_putendl_fd("minishell: Permission denied", STDERR);
		close(fd);
		return (1);
	}
	if (fd > 0)
		close(fd);
	return (0);
}

/**
 * @brief Handles redirection based on a list of redirections and an optional
 * here document file. Exits the loop (redirection chain) when an error is
 * encountered.
 *
 * @param redirects A linked list of redirections.
 * @param hd_file The name of the file for here document redirection (optional).
 */
void	handle_redirect(t_list *redirects, char *hd_file)
{
	t_list	*curr_redirect;
	t_lexer	*content;
	int		redirection;
	char	*filename;
	int		ret;

	curr_redirect = redirects;
	ret = 0;
	while (curr_redirect != NULL)
	{
		content = (t_lexer *)curr_redirect->content;
		redirection = content->token;
		filename = content->word;
		if (redirection == GREATER || redirection == GREATER_2)
			ret = handle_output(filename,
					determine_output_mode(filename, redirection));
		else if (redirection == LESS)
			ret = handle_input(filename);
		else if (hd_file)
			ret = handle_input(hd_file);
		if (ret == 1)
			exit_minishell(NULL, 1);
		curr_redirect = curr_redirect->next;
	}
}
