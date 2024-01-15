/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 10:23:00 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/15 13:59:14 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief	Generates a unique heredoc file name using an incremented static
 * variable.
 * 
 * @return	char*	unique heredoc file name
 */
char	*get_hd_file_name(void)
{
	static int	i;
	char		*str_i;
	char		*hd_file_name;

	str_i = ft_itoa(i++);
	hd_file_name = ft_strjoin("./src/heredoc/.tmp_files/.tmp_heredoc_", str_i);
	free(str_i);
	return (hd_file_name);
}

/**
 * @brief   Searches for heredoc content in the provided commands and executes
 * the heredoc process.
 * 
 * The function traverses the redirects list in the provided simple_cmd node.
 * For each heredoc redirection, <<, a temporary heredoc file is
 * generated and filled in with user input, expanding variables where
 * appropriate.
 * 
 * When << is encountered and the hd_file already exists, it is freed as to
 * make space for the new (just now encountered) heredoc because only the last
 * heredoc should be taken into account.
 * 
 * @param	simple_cmd	list containing simple command data
 * @param	env_list	list containing environment variables for expansion
 * @param	data		pointer to the t_data structure (sent to $ expander)
 */
void	process_heredoc(t_list *simple_cmd, t_data *data)
{
	t_simple_cmds	*content_simple_cmd;
	t_list			*current_redirect;
	t_lexer			*content_redirect;

	content_simple_cmd = (t_simple_cmds *) simple_cmd->content;
	current_redirect = content_simple_cmd->redirects;
	while (current_redirect)
	{
		content_redirect = (t_lexer *) current_redirect->content;
		if (content_redirect->token == LESS_2)
		{
			if (content_simple_cmd->hd_file)
				free(content_simple_cmd->hd_file);
			content_simple_cmd->hd_file = get_hd_file_name();
			create_heredoc(current_redirect, content_simple_cmd->hd_file, data);
		}
		current_redirect = current_redirect->next;
	}
}

/**
 * @brief   Executes the heredoc process for the entire command sequence.
 * 
 * The function traverses the simple commands list, sending each node into
 * the process_heredoc which takes care of running the whole heredoc process if
 * a heredoc redirection, <<, is encountered.
 * 
 * @param	data	pointer to the t_data structure
 * @return	int		returns 0 on successful execution
 */
int	heredoc(t_data *data)
{
	t_list	*current;

	current = data->simple_cmds;
	while (current)
	{
		process_heredoc(current, data);
		current = current->next;
	}
	return (0);
}
