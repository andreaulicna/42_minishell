/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 10:23:00 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/16 17:48:54 by aulicna          ###   ########.fr       */
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
	hd_file_name = ft_strjoin("./src/heredoc/.tmp_heredoc_", str_i);
	free(str_i);
	return (hd_file_name);
}

/**
 * @brief   Searches for heredoc content in the whole simple_cmds sequence and
 * executes the heredoc process for each command.
 *
 * The function traverses the simple commands list and for each noe of that it
 * traverses the redirects list via a nested while loop. 
 * 
 * For each heredoc redirection, <<, a temporary heredoc file is
 * generated and filled in with user input, expanding variables where
 * appropriate.
 * 
 * When << is encountered and the hd_file already exists, it is freed as to
 * make space for the new (just now encountered) heredoc because only the last
 * heredoc should be taken into account.
 * 
 * @param	data		pointer to the t_data structure (for simple_cmds)
 */
void	process_heredoc(t_data *data)
{
	t_list			*current_simple_cmd;
	t_simple_cmds	*content_simple_cmd;
	t_list			*current_redirect;
	t_lexer			*content_redirect;

	current_simple_cmd = data->simple_cmds;
	while (current_simple_cmd)
	{
		content_simple_cmd = (t_simple_cmds *) data->simple_cmds->content;
		current_redirect = content_simple_cmd->redirects;
		while (current_redirect)
		{
			content_redirect = (t_lexer *) current_redirect->content;
			if (content_redirect->token == LESS_2)
			{
				if (content_simple_cmd->hd_file)
					free(content_simple_cmd->hd_file);
				content_simple_cmd->hd_file = get_hd_file_name();
				create_heredoc(current_redirect, content_simple_cmd->hd_file,
					data);
			}
			current_redirect = current_redirect->next;
		}
		current_simple_cmd = current_simple_cmd->next;
	}
}

/**
 * @brief   Creates a child process that executes the heredoc process for
 * the entire command sequence.
 * 
 * The function waits for the child to exit and saved the exit status into
 * the main t_data structure.
 * 
 * @param	data	pointer to the t_data structure
 * @return	int		returns 0 on successful execution
 */

int	heredoc(t_data *data)
{
	int		pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		ft_putendl_fd("minishell: fork: Resource temporarily unavailable", 2);
		exit_current_prompt(NULL);
	}
	if (pid == 0)
	{
		process_heredoc(data);
		exit_minishell(NULL, 0);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGUSR1, handle_sigint_heredoc);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			data->exit_status = WEXITSTATUS(status);
	}
	return (0);
}
