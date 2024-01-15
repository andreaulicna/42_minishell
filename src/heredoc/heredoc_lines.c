/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_lines.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 17:48:09 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/15 13:59:13 by aulicna          ###   ########.fr       */
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
 * @param	data		pointer to the t_data structure (sent to $ expander)
 */
void	create_heredoc_dollar_line(int fd, char *line, t_data *data)
{
	t_simple_cmds	*tmp_node;

	tmp_node = malloc(sizeof(t_lexer));
	tmp_node->cmd = ft_calloc(2, sizeof(char *));
	tmp_node->cmd[0] = ft_strdup(line);
	tmp_node->cmd[1] = NULL;
	tmp_node->redirects = NULL;
	tmp_node->hd_file = NULL;
	expander_loop_dollar(tmp_node, 0, data);
	ft_putstr_fd(tmp_node->cmd[0], fd);
	free_array(tmp_node->cmd);
	free(tmp_node);
}

/**
 * @brief	Checks whether the read line is null.
 * 
 * If so, a warning message is printed and an int to indicate that the creation
 * of the heredoc should be interrupted.
 * 
 * @param	line	read line
 * @param	limiter	string that was expected to end the creation of the heredoc
 * @return	int	0 if the read line is NULL, 1 otherwise
*/
int	check_line_null(char *line, char *limiter)
{
	if (line == NULL)
	{
		ft_putstr_fd("minishell: warning: here-document delimited by ", STDOUT);
		ft_putstr_fd("end-of-file (wanted `", STDOUT);
		ft_putstr_fd(limiter, STDOUT);
		ft_putstr_fd("')\n", STDOUT);
		return (0);
	}
	return (1);
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
 * @param	data		pointer to the t_data structure (sent to $ expander)
 */
void	create_heredoc(t_list *heredoc, char *hd_file_name, t_data *data)
{
	int		fd;
	char	*line;
	char	*limiter;

	fd = open(hd_file_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
	line = readline("> ");
	limiter = ((t_lexer *) heredoc->content)->word;
	while (42)
	{
		if (!check_line_null(line, limiter))
			break ;
		else if (!ft_strncmp(line, limiter, ft_strlen(limiter))
			&& line[ft_strlen(limiter)] == '\0')
			break ;
		else if (contains_dollar(line))
			create_heredoc_dollar_line(fd, line, data);
		else
			ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
		line = readline("> ");
	}
	free(line);
	close(fd);
}
