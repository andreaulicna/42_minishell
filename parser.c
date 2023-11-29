/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:17:34 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/29 16:08:47 by aulicna          ###   ########.fr       */
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
	// end recursion when encounters |
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
		printf("will move redirect\n");
	(void) redirects;
		//move_redirect(current, redirects);	
}

t_list	*create_cmd(t_list **lexer, t_list **redirects)
{
//	char	*cmd;
	t_list	*tmp;

	separate_redirects(lexer, redirects);
	tmp = NULL;
	return(tmp);
}

t_list *lexer_to_simple_cmds(t_list **lexer)
{
	t_list *simple_cmds;
	t_list *current;
	t_lexer *content;
	t_list *redirets;


	simple_cmds = NULL;
	current = *lexer;
	while (current)
	{
		content = (t_lexer *) current->content;
		if (content->token == PIPE)
		{
			simple_cmds = create_cmd(lexer, &redirets); 
		}
		current = current->next;
	}
	return (simple_cmds);
}