/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 10:23:00 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/08 10:37:48 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief	Creates a line in the temporary heredoc file when there is a dollar
 * sign that needs to be expanded in it.
 * 
 * A temporary t_simple_cmds struct is created and the line saved as the first
 * element of the cmd variable in it. It is done this way so that the expansion
 * can be performed using the expander functions (expander.c, expander_dollar.c)
 * that take the t_simple_cmds struct as arguments.
 * 
 * Once the line is written in the file, the temporary node is freed.
 * 
 * @param	fd			file descriptor of the temporary heredoc file
 * @param	line		the line to expand and then write to the heredoc
 * @param	env_list	linked list containing environment variables
 */
void	create_heredoc_dollar_line(int fd, char *line, t_list *env_list)
{
	t_simple_cmds	*tmp_node;

	tmp_node = malloc(sizeof(t_lexer));
	tmp_node->cmd = ft_calloc(2, sizeof(char *));
	tmp_node->cmd[0] = ft_strdup(line);
	tmp_node->cmd[1] = NULL;
	tmp_node->redirects = NULL;
	tmp_node->hd_file = NULL;
	expander_loop_dollar(tmp_node, 0, env_list);
	ft_putstr_fd(tmp_node->cmd[0], fd);
	free_array(tmp_node->cmd);
	free(tmp_node);
}

/**
 * @brief	Creates a temporary heredoc file and fills it with user input.
 * 
 * The function opens and creates a heredoc file with a name detailing the order
 * the file was created in. Then it takes user input line by line and writes it
 * to the file. If the line contains a dollar sign, expansion is performed
 * in the create_heredoc_dollar_line function before writing the line
 * in the file. Each line in the file is followed by a newline.
 * 
 * @param	heredoc			list containing heredoc content
 * @param	hd_file_name	name of the temporary heredoc file
 * @param	env_list		list containing environment variables for expansion
 */
void	create_heredoc(t_list *heredoc, char *hd_file_name, t_list *env_list)
{
	int		fd;
	char	*line;
	t_lexer	*content_heredoc;

	fd = open(hd_file_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
	line = readline("> ");
	content_heredoc = (t_lexer *) heredoc->content;
	while (line && ft_strncmp(content_heredoc->word, line,
			ft_strlen(line)))
	{
		if (contains_dollar(line))
			create_heredoc_dollar_line(fd, line, env_list);
		else
			ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
		line = readline("> ");
	}
	free(line);
	close(fd);
}

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
 */
void	process_heredoc(t_list *simple_cmd, t_list *env_list)
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
			create_heredoc(current_redirect, content_simple_cmd->hd_file,
				env_list);
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
		process_heredoc(current, data->env_list);
		current = current->next;
	}
	return (0);
}
