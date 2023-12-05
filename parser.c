/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:17:34 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/05 15:27:20 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"

// needs updating later
/**
 * @brief 	Handles syntax error according to the error code sent.
 * 
 * @param	code	integer representing the error code
 * 					1: nothing following a redirection (or a pipe?)
 * @return	int		returns the exit status code (EXIT_FAILURE)
 */

int	error_handler(int code)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (code == 1)
		ft_putstr_fd("syntax error near unexpected token `newline'\n",
			STDERR_FILENO);
	return (EXIT_FAILURE);
}


 /**
 * @brief	Handles syntax error when encountering an unexpected double token.
 * 
 * @param	token	token from the defined enum representing different symbols
 * @return	int		returns the exit status code (EXIT_FAILURE)
 */

int	error_parser_double_token(t_tokens token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	if (token == PIPE)
		ft_putstr_fd("|", STDERR_FILENO);
	else if (token == LESS)
		ft_putstr_fd("<", STDERR_FILENO);
	else if (token == LESS_2)
		ft_putstr_fd("<<", STDERR_FILENO);
	else if (token == GREATER)
		ft_putstr_fd(">", STDERR_FILENO);
	else if (token == GREATER_2)
		ft_putstr_fd(">>", STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	return (EXIT_FAILURE);
}

/**
 * @brief	Moves a redirection node to the redirects list.
 * 
 * The node that was moved still stays in the lexer list and is deleted (freed)
 * in the function which calls this function, separate_redirects, once
 * the control is returned to it.
 * 
 * @param	node		pointer to the node to be moved
 * @param	redirects	pointer to the list to which the node will be added
 */

void	move_redirect_to_redirects_list(t_list *node, t_list **redirects)
{
	t_list	*node_move;
	t_list	*node_new;
	t_lexer	*content_move;
	t_lexer	*content_move_word;
	t_lexer	*content_new;

	node_move = node;
	content_move = (t_lexer *) node_move->content;
	content_new = malloc(sizeof(t_lexer));
	content_new->id = content_move->id;
	content_new->token = content_move->token;
	content_move_word = (t_lexer *) node_move->next->content;
	content_new->word = ft_strdup(content_move_word->word);
	node_new = ft_lstnew(content_new);
	ft_lstadd_back(redirects, node_new);
}

/**
 * @brief	Frees a node from the lexer list based on the provided ID.
 * 
 * It takes into account the structure of the lexer list that is defined
 * as a t_list with its void *content pointing to the t_lexer struct. And so,
 * besides the content, it first needs to free the char *word that has also
 * been malloced.
 * 
 * @param	lexer	pointer to the lexer list
 * @param	id		ID of the node to be removed
 */

void	free_lexer_node(t_list **lexer, int id)
{
	t_list	*current;
	t_list	*previous;
	t_lexer	*content;

	current = *lexer;
	previous = NULL;
	content = (t_lexer *) current->content;
	while (current && content->id != id)
	{
		previous = current;
		current = current->next;
		content = (t_lexer *) current->content;
	}
	if (previous)
		previous->next = current->next;
	else
		*lexer = current->next;
	if (content->word)
		free(content->word);
	free(content);
	content = NULL;
	free(current);
	current = NULL;
}

/**
 * @brief	Handles the separation on the redirections from the lexer list
 * into the redirects list that is eventually saved within the simple_cmds list.
 * 
 * Function line by line:
 * while	skips through notes that are not tokens
 * if		ends recursion when either the end of a pipe is encountered
 * if		throws an error if there is nothing following a token
 * if		throws an error if the next node is a token too (double token error)
 * if		moves a redirection to the redirects list and deletes (frees) it 
 * 			(it and the following one to keep the redirection and the "place"
 * 			to redirect output together) once such a valid node is found
 * 
 * The function is called recursively so that it exits once the current node
 * is NULL (indicates the end of the lexer list) or a PIPE is encountered. 
 * 
 * @param	lexer		pointer to the lexer list
 * @param	redirects	pointer to the redirects list
 */

void	separate_redirects(t_list **lexer, t_list **redirects)
{
	t_list	*current;
	t_lexer	*content;

	current = *lexer;
	content = (t_lexer *) current->content;
	while (current && !content->token)
	{
		current = current->next;
		if (current)
			content = (t_lexer *) current->content;
	}
	if (!current || content->token == PIPE)
		return ;
	if (!current->next)
		error_handler(1);
	content = (t_lexer *) current->next->content;
	if (content->token)
		error_parser_double_token(content->token);
	content = (t_lexer *) current->content;
	if (content->token == LESS || content->token == LESS_2
		|| content->token == GREATER || content->token == GREATER_2)
	{
		move_redirect_to_redirects_list(current, redirects);
		free_lexer_node(lexer, content->id + 1);
		free_lexer_node(lexer, content->id);
	}
	separate_redirects(lexer, redirects);
}

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
	content_cmd->cmd = cmd;
	content_cmd->redirects = redirects;
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
