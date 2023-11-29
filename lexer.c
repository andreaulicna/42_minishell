/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:07:57 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/29 12:19:13 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"

t_tokens	is_token(char *check)
{
	unsigned int	len;

	len = ft_strlen(check);
	if (!ft_strncmp("|", check, len))
		return PIPE;
	else if (!ft_strncmp("<", check, len))
		return LESS;
	else if (!ft_strncmp("<<", check, len))
		return LESS_2;
	else if (!ft_strncmp(">", check, len))
		return GREATER;
	else if (!ft_strncmp(">>", check, 2))
		return GREATER_2;
	return (0);
}

t_list *input_arr_to_lexer_list(char **input_split)
{
	t_list *lexer;
	t_list *current;
	t_lexer *content;
	int	i;

	lexer = NULL;
	current = NULL;
	i = 0;
	while (input_split[i])
	{
		content = malloc(sizeof(t_lexer));
		content->id = i;
		if (is_token(input_split[i]))
		{
			content->word = NULL;
			content->token = is_token(input_split[i]);
		}
		else
		{
			content->word = ft_strdup(input_split[i]);
			content->token = 0;
		}
		current = ft_lstnew(content);
		ft_lstadd_back(&lexer, current);
		i++;
	}
	return (lexer);
}