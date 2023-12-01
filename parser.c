/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:17:34 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/01 14:06:16 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"

int	error_parser_nothing_after_token(int code)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (code == 1)
		ft_putstr_fd("syntax error near unexpected token `newline'\n",
			STDERR_FILENO);
	return (EXIT_FAILURE);
}

int	error_parser_double_token(t_tokens token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	if (token == LESS)
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

void	move_redirect_to_redirects_list(t_list *node, t_list **redirects)
{
	t_list	*node_move;
	t_list	*node_new;
	t_lexer	*content_move;
	t_lexer *content_move_word;
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

void	free_lexer_node(t_list **lexer, int id)
{
	t_list *current;
	t_list *previous;
	t_lexer *content;

	current = *lexer;
	previous = NULL;

	content = (t_lexer *) current->content;
	while (current && content->id != id)
	{
		previous = current;
		current = current->next;
		content = (t_lexer *) current->content;
	}
	previous->next = current->next;
	if (content->word)
		free(content->word);
	free(content);
	content = NULL;
	free(current);
	current = NULL;
}

void	separate_redirects(t_list **lexer, t_list **redirects)
{
	t_list	*current;
	t_lexer	*content;

	current = *lexer;
	content = (t_lexer *) current->content;
	// skip through nodes that are not tokens
	while (current && !content->token)
	{
		current = current->next;
		content = (t_lexer *) current->content;
	}
	// end recursion when end the end of lexer list or encounters |
	if (!current || content->token == PIPE)
		return ;
	// error if there is nothing after the token
	if (!current->next)
		error_parser_nothing_after_token(1);
	// next node is token too
	content = (t_lexer *) current->next->content;
	if (content->token)
		error_parser_double_token(content->token);
	// check current node token and move redirection
	content = (t_lexer *) current->content;
	if (content->token == LESS || content->token == LESS_2
		|| content->token == GREATER || content->token == GREATER_2)
	{
		move_redirect_to_redirects_list(current, redirects);
		free_lexer_node(lexer, content->id + 1);
		free_lexer_node(lexer, content->id);
		printf("jou\n");
	}
	separate_redirects(lexer, redirects);
}

void	create_cmd(t_list **lexer, t_list **redirects)
{
	separate_redirects(lexer, redirects);
}

t_list *lexer_to_simple_cmds(t_list **lexer)
{
	t_list *simple_cmds;
	t_list *node_simple_cmds;
	t_list *current;
	t_lexer *content;
	t_list *redirects;
	t_simple_cmds *content_cmd;


	simple_cmds = NULL;
	redirects = NULL;
	current = *lexer;
	while (current)
	{
		content = (t_lexer *) current->content;
		if (content->token == PIPE)
		{
			printf("token\n");
			create_cmd(lexer, &redirects); 
		}
		current = current->next;
	}
	content_cmd = malloc(sizeof(t_lexer));
	content_cmd->cmd = NULL;
	content_cmd->redirects = redirects;
	//content_cmd->builtin = NULL;
	node_simple_cmds = ft_lstnew(content_cmd);
	ft_lstadd_back(&simple_cmds, node_simple_cmds);
	return (simple_cmds);
}