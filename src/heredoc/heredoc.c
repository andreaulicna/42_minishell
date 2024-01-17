/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 10:23:00 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/17 12:25:06 by aulicna          ###   ########.fr       */
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
 * @brief	Creates a child process that manages the saving of heredoc input.
 * 
 * A temporary heredoc file is generated and filled in with user input,
 * expanding variables where appropriate.
 * 
 * The function waits for the child to exit and saves the exit status into
 * the main t_data structure.
 * 
 * Signal lines (parent process):
 * 1. SIGINT: Ignores the SIGINT signal, so that the handle_sigint handler
 * doesn't get triggered in the parent process for now as there is a child
 * process running that has its own handler for this signal.
 * 
 * 2. SIGUSR1: Sets up the signal handler for SIGUSR1 to handle_sigint_heredoc
 * Thanks to that when the child process sends it to indicate that the heredoc
 * process has been interrupted by SIGINT (Ctrl + C), the parent process saves
 * this information in the global variable g_signal. This variable is then
 * checked in the minishell_loop before the execution is triggered and if
 * it is set to SIGUSR1, the execution won't be triggered because SIGINT
 * received during the heredoc process cancels the whole command.
 * 
 * 
 * @param	current_redirect		list containing heredoc content
 * @param	content_simle_cmd		pointer to the content of current_simple_cmd
 * 									to access the name of the temp heredoc file
 * @param	data					pointer to the t_data structure
 * 									(for exit_status and to send to 
 * 									create_heredoc and then expander)
 */
void	process_heredoc(t_data *data, t_list *current_redirect,
	t_simple_cmds *content_simple_cmd)
{
	int				pid;
	int				status;

	pid = fork();
	if (pid == 0)
	{
		create_heredoc(current_redirect, content_simple_cmd->hd_file,
			data);
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
				process_heredoc(data, current_redirect, content_simple_cmd);
			}
			current_redirect = current_redirect->next;
		}
		current_simple_cmd = current_simple_cmd->next;
	}
	return (0);
}
