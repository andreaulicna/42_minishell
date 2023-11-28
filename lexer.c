/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:07:57 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/28 15:56:42 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"

t_tokens	is_token(char *check)
{
	if (!ft_strncmp("|", check, 1))
		return PIPE;
	else if (!ft_strncmp("<", check, 1))
		return LESS;
	return (0);
}

t_lexer	*ft_lexer_new(unsigned int id, char *word, t_tokens token)
{
	t_lexer	*tmp;

	tmp = malloc(sizeof(t_lexer));
	if (tmp)
	{
		tmp->id = id;
		tmp->word = word;
		tmp->token = token;
		tmp->next = NULL;
	}
	return (tmp);
}

int	ft_lexer_size(t_lexer *lexer)
{
	int	i;

	i = 0;
	while (lexer)
	{
		lexer = lexer->next;
		i++;
	}
	return (i);
}

t_lexer	*ft_lexer_last(t_lexer *lexer)
{
	int	size;

	size = ft_lexer_size(lexer);
	while (size > 1)
	{
		lexer = lexer->next;
		size--;
	}
	return (lexer);
}

void	ft_lexer_add_back(t_lexer **lexer, t_lexer *new)
{
	t_lexer	*last;

	last = ft_lexer_last(*lexer);
	if (!last)
		*lexer = new;
	else
		last->next = new;
}

t_lexer *input_arr_to_lexer_list(char **input_split) 
{
	t_lexer	*lexer;
	t_lexer	*current;
	int	i;

	lexer = NULL;
	current = NULL;
	i = 0;
	while (input_split[i])
	{
		if (is_token(input_split[i]))
			current = ft_lexer_new(i, NULL, is_token(input_split[i]));
		else
			current = ft_lexer_new(i, input_split[i], 0);
		ft_lexer_add_back(&lexer, current);
	//	printf("word_l: %s\n", current->word);
	//	printf("token_l: %d\n", current->token);
		i++;
	}
//	printf("word_ll: %s\n", lexer->word);
//	printf("token_ll: %d\n", lexer->token);
	return (lexer);
}
