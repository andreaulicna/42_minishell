/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirects.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 10:59:11 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/23 12:34:52 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

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
 * @brief	Moves a redirection node to the redirects list. Once the node
 * is moved, this function also deletes (frees) it from the lexer list.
 * 
 * @param	node		pointer to the node to be moved
 * @param	redirects	pointer to the list to which the node will be added
 */
void	move_redirect_to_redirects_list(t_list **lexer, t_list *node,
	t_list **redirects)
{
	t_list	*node_move;
	t_list	*node_new;
	t_lexer	*content_move;
	t_lexer	*content_move_word;
	t_lexer	*content_new;

	node_move = node;
	content_move = (t_lexer *) node_move->content;
	content_new = malloc(sizeof(t_lexer));
	if (!content_new)
		exit_minishell(NULL, EXIT_MALLOC);
	content_new->id = content_move->id;
	content_new->token = content_move->token;
	content_move_word = (t_lexer *) node_move->next->content;
	content_new->word = ft_strdup(content_move_word->word);
	node_new = ft_lstnew(content_new);
	ft_lstadd_back(redirects, node_new);
	free_lexer_node(lexer, content_move->id + 1);
	free_lexer_node(lexer, content_move->id);
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
	if (!current)
		return ;
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
		move_redirect_to_redirects_list(lexer, current, redirects);
	separate_redirects(lexer, redirects);
}
