/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 07:49:03 by vbartos           #+#    #+#             */
/*   Updated: 2024/01/25 17:44:04 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

// /**
//  * @brief Handles redirecting standard output to a file, overwriting the file
//  * if it exists.
//  * 
//  * @param filename The name of the file to redirect the output to.
//  */
// void handle_output_single(t_data *data, char *filename)
// {
// 	int fd;
	
// 	if (access(filename, W_OK) == -1)
// 	{
// 		ft_putendl_fd("minishell: Permission denied", STDERR);
// 		data->exit_status = 1;
// 		exit_current_prompt(data);
// 	}
// 	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
// 	if (fd < 0)
// 	{
// 		ft_putstr_fd("minishell: ", STDERR);
// 		ft_putstr_fd(filename, STDERR);
// 		ft_putendl_fd(": Error attempting to open file", STDERR);
// 		data->exit_status = 1;
// 		return ;
// 	}
// 	if (fd > 0 && dup2(fd, STDOUT) < 0)
// 	{
// 		ft_putendl_fd("minishell: pipe error", STDERR);
// 		data->exit_status = 1;
// 		return ;
// 	}
// 	if (fd > 0)
// 		close(fd);
// }

// /**
//  * @brief Handles redirecting standard output to a file, appending the output
//  * to the file if it exists.
//  * 
//  * @param filename The name of the file to redirect the output to.
//  */
// void handle_output_append(t_data *data, char *filename)
// {
// 	int fd;
	
// 	if (access(filename, W_OK) == -1)
// 	{
// 		ft_putendl_fd("minishell: Permission denied", STDERR);
// 		data->exit_status = 1;
// 		exit_current_prompt(data);
// 	}
// 	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
// 	if (fd < 0)
// 	{
// 		ft_putstr_fd("minishell: ", STDERR);
// 		ft_putstr_fd(filename, STDERR);
// 		ft_putendl_fd(": Error attempting to open file", STDERR);
// 		data->exit_status = 1;
// 		return ;
// 	}
// 	if (fd > 0 && dup2(fd, STDOUT) < 0)
// 	{
// 		ft_putendl_fd("minishell: pipe error", STDERR);
// 		data->exit_status = 1;
// 		return ;
// 	}
// 	if (fd > 0)
// 		close(fd);
// }

// /**
//  * @brief Handles redirecting standard input from a file or heredoc.
//  * 
//  * @param filename The name of the file to redirect the input from.
//  */
// void handle_input(t_data *data, char *filename)
// {
// 	int fd;
	
// 	fd = open(filename, O_RDONLY);
// 	if (fd < 0)
// 	{
// 		ft_putstr_fd("minishell: ", STDERR);
// 		ft_putstr_fd(filename, STDERR);
// 		ft_putendl_fd(": No such file or directory", STDERR);
// 		data->exit_status = 1;
// 		exit_current_prompt(data);
// 	}
// 	if (access(filename, R_OK) == -1)
// 	{
// 		ft_putendl_fd("minishell: Permission denied", STDERR);
// 		data->exit_status = 1;
// 		exit_current_prompt(data);
// 	}
// 	if (fd > 0 && dup2(fd, STDIN) < 0)
// 	{
// 		ft_putendl_fd("minishell: pipe error", STDERR);
// 		data->exit_status = 1;
// 		exit_current_prompt(data);
// 	}
// 	if (fd > 0)
// 		close(fd);
// }

// /**
//  * @brief Handles the redirections specified in the given list.
//  * 
//  * This function iterates over the list of redirects and performs the correct
//  * action based on the type of redirection. The supported redirection types are:
//  * - Output redirection (single '>' operator)
//  * - Output redirection (append '>>' operator)
//  * - Input redirection (single '<' operator)
//  * - Input redirection (heredoc '<<' operator)
//  * 
//  * @param redirects The list of redirects to handle.
//  */
// void handle_redirect(t_data *data, t_list *redirects, char *hd_file)
// {
// 	t_list	*curr_redirect = redirects;
// 	t_lexer	*content;
// 	int		redirection;
// 	char	*filename;
	
// 	while (curr_redirect != NULL)
// 	{
// 		content = (t_lexer *)curr_redirect->content;
// 		redirection = content->token;
// 		filename = content->word;
// 		if (redirection == GREATER)
// 			handle_output_single(data, filename);
// 		else if (redirection == GREATER_2)
// 			handle_output_append(data, filename);
// 		else if (redirection == LESS)
// 			handle_input(data, filename);
// 		else if (hd_file)
// 			handle_input(data, hd_file);
// 		curr_redirect = curr_redirect->next;
// 	}
// }

/**
 * @brief Handles redirecting standard output to a file, overwriting the file
 * if it exists.
 * 
 * @param filename The name of the file to redirect the output to.
 */
void handle_output_single(t_data *data, char *filename)
{
	int fd;
	
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(filename, STDERR);
		ft_putendl_fd(": Permission denied", STDERR);
		data->exit_status = 1;
		return ;
	}
	if (fd > 0 && dup2(fd, STDOUT) < 0)
	{
		ft_putendl_fd("minishell: pipe error", STDERR);
		data->exit_status = 1;
		return ;
	}
	if (fd > 0)
		close(fd);
}

/**
 * @brief Handles redirecting standard output to a file, appending the output
 * to the file if it exists.
 * 
 * @param filename The name of the file to redirect the output to.
 */
void handle_output_append(char *filename)
{
	int fd;
	
	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
	dup2(fd, STDOUT);
	close(fd);
}

/**
 * @brief Handles redirecting standard input from a file or heredoc.
 * 
 * @param filename The name of the file to redirect the input from.
 */
void handle_input(t_data *data, char *filename)
{
	int fd;
	
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR);
		ft_putstr_fd(filename, STDERR);
		ft_putendl_fd(": No such file or directory", STDERR);
		data->exit_status = 1;
		exit_current_prompt(data);
	}
	if (fd > 0 && dup2(fd, STDIN) < 0)
	{
		ft_putendl_fd("minishell: pipe error", STDERR);
		data->exit_status = 1;
		exit_current_prompt(data);
	}
	if (access(filename, R_OK) == -1)
	{
		ft_putendl_fd("minishell: Permission denied", STDERR);
		data->exit_status = 1;
		exit_current_prompt(data);
	}
	if (fd > 0)
		close(fd);
}

/**
 * @brief Handles the redirections specified in the given list.
 * 
 * This function iterates over the list of redirects and performs the correct
 * action based on the type of redirection. The supported redirection types are:
 * - 4: Output redirection (single '>' operator)
 * - 5: Output redirection (append '>>' operator)
 * - 2: Input redirection (single '<' operator)
 * - 3: Input redirection (heredoc '<<' operator)
 * 
 * @param redirects The list of redirects to handle.
 */
void handle_redirect(t_data *data, t_list *redirects, char *hd_file)
{
	t_list	*curr_redirect = redirects;
	t_lexer	*content;
	int		redirection;
	char	*filename;
	
	while (curr_redirect != NULL)
	{
		content = (t_lexer *)curr_redirect->content;
		redirection = content->token;
		filename = content->word;
		if (redirection == GREATER)
			handle_output_single(data, filename);
		else if (redirection == GREATER_2)
			handle_output_append(filename);
		else if (redirection == LESS)
			handle_input(data, filename);
		else if (hd_file)
			handle_input(data, hd_file);
		curr_redirect = curr_redirect->next;
	}
}