/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 10:28:47 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/12 12:01:32 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	print_lexer(t_list **lexer)
{
	t_lexer	*content;
	t_list	*current;

	current = *lexer;
	while (current != NULL)
	{
		content = (t_lexer *)(current->content);
		if (content->token)
			ft_printf("token: %d\n", content->token);
		else
			ft_printf("word: %s\n", content->word);
		current = current->next;
	}
}

void	print_redirects(t_list *current_redirect)
{
	t_lexer			*content_redirects;

	printf("\n***Redirects***\n");
	while (current_redirect)
	{
		content_redirects = (t_lexer *) current_redirect->content;
		printf("Redirect token: %d\n", content_redirects->token);
		printf("Redirect word: %s\n", content_redirects->word);
		current_redirect = current_redirect->next;
	}
}

void	print_simple_cmds(t_list **simple_cmds)
{
	t_list			*current;
	t_simple_cmds	*content_simple_cmds;
	t_list			*current_redirect;
	int				i;
	int				order;

	current = *simple_cmds;
	order = 0;
	while (current)
	{
		printf("%s %d\n", "SIMPLE CMDS", order);
		content_simple_cmds = (t_simple_cmds *) current->content;
		current_redirect = content_simple_cmds->redirects;
		printf("\n***Cmds***\n");
		i = 0;
		while (content_simple_cmds->cmd[i])
		{
			printf("%s\n", content_simple_cmds->cmd[i]);
			i++;
		}
		printf("\n***Redirects***\n");
		print_redirects(current_redirect);
		current = current->next;
		order++;
	}
}
