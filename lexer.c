/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:07:57 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/06 11:46:24 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"

/**
 * @brief	Identifies a token from a given string.
 * 
 * This function checks if the given string matches any predefined token
 * ("|", "<", "<<", ">", ">>"). If a match is found, it returns
 * the corresponding token enum value as defined in the s_tokens struct;
 * otherwise, it returns 0.
 * 
 * @param	check		string to be checked against known token strings
 * @return	t_tokens	returns the corresponding token enum value if found;
 * otherwise, returns 0
 */

t_tokens	is_token(char *check)
{
	unsigned int	len;

	len = ft_strlen(check);
	if (!ft_strncmp("|", check, len))
		return (PIPE);
	else if (!ft_strncmp("<", check, len))
		return (LESS);
	else if (!ft_strncmp("<<", check, len))
		return (LESS_2);
	else if (!ft_strncmp(">", check, len))
		return (GREATER);
	else if (!ft_strncmp(">>", check, 2))
		return (GREATER_2);
	return (0);
}

/**
 * @brief	Helper function for the input_arr_to_lexer_list function. Treats
 * the creation of a token node ("|", "<", "<<", ">", ">>").
 * 
 * @param	content	pointer to the content of the node to assign the token to	
 * @param	token	token to assign
*/

void	assign_token(t_lexer *content, t_tokens token)
{
	content->word = NULL;
	content->token = token;
}

/**
 * @brief	Helper function for the input_arr_to_lexer_list function. Treats
 * the creation of a word node.
 *  
 * @param	content	pointer to the content of the node to assign the word to	
 * @param	token	word to assign
*/

void	assign_word(t_lexer *content, char *word)
{
	content->word = ft_strdup(word);
	content->token = 0;
}

/**
 * @brief	Converts an input string array into a lexer-linked list where each
 * node represents either a token or a word from the input.
 * 
 * For each node of the lexer list, either the word or the token is filled in.
 * 
 * @param	input_split	array of strings obtained from ft_split_minishell
 * @return	t_list*		returns a pointer to the head of the lexer-linked list.
 */

t_list	*input_arr_to_lexer_list(char **input_split)
{
	t_list	*lexer;
	t_list	*current;
	t_lexer	*content;
	int		i;

	lexer = NULL;
	current = NULL;
	i = 0;
	while (input_split[i])
	{
		content = malloc(sizeof(t_lexer));
		content->id = i;
		if (is_token(input_split[i]))
			assign_token(content, is_token(input_split[i]));
		else
			assign_word(content, input_split[i]);
		current = ft_lstnew(content);
		ft_lstadd_back(&lexer, current);
		i++;
	}
	return (lexer);
}
