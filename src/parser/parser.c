/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:17:34 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/14 12:38:38 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief	Gets the length of a command within the lexer list. Everything up
 * until a pipe or the end of the lexer list is counted.
 * 
 * @param	lexer	pointer to the lexer list
 * @return	int		returns the length of the command
 */

int	ft_cmd_len(t_list **lexer)
{
	int		i;
	t_list	*current;
	t_lexer	*content;

	if (!lexer)
		return (0);
	current = *lexer;
	content = (t_lexer *) current->content;
	i = 0;
	while (current && content->token != PIPE)
	{
		i++;
		current = current->next;
		if (current)
			content = (t_lexer *) current->content;
	}
	return (i);
}

/**
 * @brief	Creates the command (cmd) needed for the simple_cmds list's content,
 * as defined in the t_simple_cmds struct, from the lexer list. That is done
 * by assembling appropriate nodes from the lexer list into a 2D array.
 * 
 * It uses the cmd_len that is a product of the ft_cmd_len function to loop
 * through the lexer list. For each word it encounters, it creates a copy of it
 * that is stored in the corresponding place in char **cmd. Then it frees that
 * node from the lexer list. The freed node used to be the first one in
 * the lexer list, hence current needs to be assigned to the beginning
 * of the lexer again.
 * 
 * @param	lexer	pointer to the lexer list
 * @param	cmd		array of strings representing the command
 * @param	cmd_len	the length of the command
 */

void	create_cmd(t_list **lexer, char **cmd, int cmd_len)
{
	int		i;
	t_list	*current;
	t_lexer	*content;

	i = 0;
	current = *lexer;
	while (cmd_len > 0)
	{
		content = (t_lexer *) current->content;
		if (content->word)
		{
			cmd[i] = ft_strdup(content->word);
			free_lexer_node(lexer, content->id);
			current = *lexer;
		}
		cmd_len--;
		i++;
	}
	cmd[i] = NULL;
}

/**
 * @brief Creates simple commands by separating commands from the lexer list.
 * 
 * The function prepares the content of a new simple_cmds, defined
 * as the t_simple_cmds struct, node by calling the corresponding functions.
 * Then it initializes the node and adds it to the back of the simple_cmds list.
 * 
 * @param	lexer		pointer to the lexer list
 * @param	simple_cmds	pointer to the list of simple commands
 */

void	create_simple_cmds(t_list **lexer, t_list **simple_cmds)
{
	int				cmd_len;
	char			**cmd;
	t_list			*node_simple_cmds;
	t_list			*redirects;
	t_simple_cmds	*content_cmd;

	redirects = NULL;
	separate_redirects(lexer, &redirects);
	cmd_len = ft_cmd_len(lexer);
	cmd = ft_calloc(cmd_len + 1, sizeof(char *));
	create_cmd(lexer, cmd, cmd_len);
	content_cmd = malloc(sizeof(t_lexer));
	if (!content_cmd)
		exit_minishell(NULL, EXIT_MALLOC);
	content_cmd->cmd = cmd;
	content_cmd->redirects = redirects;
	content_cmd->hd_file = NULL;
	node_simple_cmds = ft_lstnew(content_cmd);
	ft_lstadd_back(simple_cmds, node_simple_cmds);
}

/**
 * @brief	Converts lexer list to a list of simple commands.
 * 
 * Function line by line:
 * if		throws an error if a pipe is encountered as the first node
 * 			of the lexer list (double token error)
 * while	loops through the lexer list creating a node in the simple_cmds list
 * 			for each collection of words divided by a pipe or the end
 * 			of the lexer list
 * if (in while)	the create_simple_cmds function threats nodes up until
 * 					the pipe (or the end of the lexer list), therefore in
 * 					the next go of the while loop, the pipe node needs to be
 * 					removed and then only in the following run another node
 * 					of the simple_cmds list is created	
 * 
 * @param	lexer	pointer to the lexer list
 * @return	t_list*	returns the list of simple commands
 */

t_list	*lexer_to_simple_cmds(t_list **lexer)
{
	t_list	*simple_cmds;
	t_list	*current;
	t_lexer	*content;

	simple_cmds = NULL;
	current = *lexer;
	content = (t_lexer *) current->content;
	if (content->token == PIPE)
		error_parser_double_token(content->token);
	while (current)
	{
		content = (t_lexer *) current->content;
		if (content->token == PIPE)
		{
			free_lexer_node(lexer, content->id);
			current = *lexer;
			continue ;
		}
		create_simple_cmds(lexer, &simple_cmds);
		current = *lexer;
	}
	return (simple_cmds);
}
