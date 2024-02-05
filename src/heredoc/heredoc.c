/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 10:23:00 by aulicna           #+#    #+#             */
/*   Updated: 2024/02/06 00:43:37 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief	Generates a unique heredoc file name using an incremented static
 * variable.
 * 
 * @return	char*	unique heredoc file name
 */
static char	*get_hd_file_name(void)
{
	static int	i;
	char		*str_i;
	char		*hd_file_name;

	i++;
	str_i = ft_itoa(i);
	hd_file_name = ft_strjoin(".tmp_heredoc_", str_i);
	free(str_i);
	return (hd_file_name);
}

/**
 * @brief	Creates a child process that manages the saving of heredoc input.
 * 
 * A temporary heredoc file is generated and filled in with user input,
 * expanding variables where appropriate.
 * 
 * The function waits for the child to exit and saves the exit status into
 * the main t_data structure.
 * 
 * Signals management:
 * In the child process, SIGINT handler prints a newline and exits with exit
 * status of 130.
 * In the parent process, SIGINT handler sets g_signal to SIGINT so that this
 * value can be checked before launching exec (which shouldn't run if
 * the heredoc process was interrupted with SIGINT).
 * 
 * @param	current_redirect		list containing heredoc content
 * @param	content_simle_cmd		pointer to the content of current_simple_cmd
 * 									to access the name of the temp heredoc file
 * @param	data					pointer to the t_data structure
 * 									(for exit_status and to send to 
 * 									create_heredoc and then expander)
 */
static void	process_heredoc(t_data *data, t_list *current_redirect,
	t_simple_cmds *content_simple_cmd)
{
	int				pid;
	int				status;

	fork_process(&pid);
	if (pid == 0)
	{
		signal(SIGINT, handle_sigint_heredoc);
		create_heredoc(current_redirect, content_simple_cmd->hd_file,
			data);
		exit_minishell(NULL, 0);
	}
	else
	{
		signal(SIGINT, handle_sigint_when_child_running);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			data->exit_status = WEXITSTATUS(status);
		else if (WTERMSIG(status) == g_signal)
			data->exit_status = 130;
	}
}

/**
 * @brief   Searches for heredoc content in the whole simple_cmds sequence and
 * executes the heredoc process for each command.
 *
 * The function traverses the simple commands list and for each one of that it
 * traverses the redirects list via a nested while loop. 
 * 
 * For each heredoc redirection, <<, the process_heredoc function is called.
 * 
 * When << is encountered and the hd_file already exists, it is freed as to
 * make space for the new (just now encountered) heredoc because only the last
 * heredoc should be taken into account.
 * 
 * @param	data	pointer to the t_data structure
 * @return	int		returns 0 on successful execution
 */
int	heredoc(t_data *data)
{
	t_list			*current_simple_cmd;
	t_simple_cmds	*content_simple_cmd;
	t_list			*current_redirect;
	t_lexer			*content_redirect;

	current_simple_cmd = data->simple_cmds;
	while (current_simple_cmd)
	{
		content_simple_cmd = (t_simple_cmds *) current_simple_cmd->content;
		current_redirect = content_simple_cmd->redirects;
		while (current_redirect)
		{
			content_redirect = (t_lexer *) current_redirect->content;
			if (content_redirect->token == LESS_2)
			{
				if (content_simple_cmd->hd_file)
					free(content_simple_cmd->hd_file);
				content_simple_cmd->hd_file = get_hd_file_name();
				process_heredoc(data, current_redirect, content_simple_cmd);
			}
			current_redirect = current_redirect->next;
		}
		current_simple_cmd = current_simple_cmd->next;
	}
	return (0);
}
