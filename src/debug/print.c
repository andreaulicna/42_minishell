/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 10:28:47 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/11 15:48:30 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	print_input_split(char **input_split)
{
	int	i;

	i = 0;
	while (input_split[i])
	{
		printf("%s\n", input_split[i]);
		i++;
	}
}

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
	t_simple_cmds	*content_simple_cmd;
	int				i;
	int				order;

	current = *simple_cmds;
	order = 0;
	while (current)
	{
		printf("\n%s %d\n\n%s\n", "SIMPLE CMDS", order, "***Cmds***");
		content_simple_cmd = (t_simple_cmds *) current->content;
		i = 0;
		while (content_simple_cmd->cmd[i])
		{
			printf("%s\n", content_simple_cmd->cmd[i]);
			i++;
		}
		print_redirects(content_simple_cmd->redirects);
		printf("\n***Hd_file***\n");
		if (content_simple_cmd->hd_file)
			printf("%s\n", content_simple_cmd->hd_file);
		printf("\n");
		current = current->next;
		order++;
	}
}
