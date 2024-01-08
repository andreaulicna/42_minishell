/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 07:49:03 by vbartos           #+#    #+#             */
/*   Updated: 2024/01/08 13:01:09 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

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
void handle_redirect(t_list *redirects)
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
		if (redirection == 4)
			handle_output_single(filename);
		else if (redirection == 5)
			handle_output_append(filename);
		else if (redirection == 2)
			handle_input_single(filename);
		else if (redirection == 3)
			handle_input_heredoc(filename);
		curr_redirect = curr_redirect->next;
	}
}

/**
 * @brief Handles redirecting standard output to a file, overwriting the file
 * if it exists.
 * 
 * @param filename The name of the file to redirect the output to.
 */
void handle_output_single(char *filename)
{
	int fd;
	
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	dup2(fd, STDOUT);
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
 * @brief Handles redirecting standard input from a file.
 * 
 * @param filename The name of the file to redirect the input from.
 */
void handle_input_single(char *filename)
{
	int fd;
	
	fd = open(filename, O_RDONLY);
	dup2(fd, STDIN);
	close(fd);
}

/**
 * @brief Handles redirecting standard input from a file using a here document.
 * 
 * @param filename The name of the file to redirect the input from.
 */
void handle_input_heredoc(char *filename)
{
	int fd;
	
	fd = open(filename, O_RDONLY);
	dup2(fd, STDIN);
	close(fd);
}
