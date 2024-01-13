/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_space_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 17:26:37 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/11 18:56:08 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief	Checks if a string contains any space character.
 *
 * @param	s	string to check
 * @return	int	1 if the string contains space, 0 otherwise
 */
int	contains_space(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief	Determines the type of token present at the beginning of a string
 * without spaces.
 *
 * @param	s	string to check
 * @return	t_tokens	corresponding token type (t_tokens enum) or 0
 */
t_tokens	contains_token_with_no_space(char *s)
{
	if (is_token(s))
		return (0);
	else if (s[0] == '|' && s[1])
		return (PIPE);
	else if (s[0] == '<' && s[1] == '<' && s[2])
		return (LESS_2);
	else if (s[0] == '<' && s[1])
		return (LESS);
	else if (s[0] == '>' && s[1] == '>' && s[2])
		return (GREATER_2);
	else if (s[0] == '>' && s[1])
		return (GREATER);
	return (0);
}

/**
 * @brief 	Fills the new input split array with the appropriate token and
 * the remaining string.
 *
 * @param	token 			type of token to be added as a separate str in array
 * @param	new_input_split	new input_split
 * @param	i 				pointer to the current new_i of new_input_split
 * @param	word			remaining string after the token
 */
void	fill_token(t_tokens token, char **new_input_split, int *i, char *word)
{
	if (token == PIPE)
		new_input_split[*i] = ft_strdup("|");
	else if (token == LESS_2)
		new_input_split[*i] = ft_strdup("<<");
	else if (token == LESS)
		new_input_split[*i] = ft_strdup("<");
	else if (token == GREATER)
		new_input_split[*i] = ft_strdup(">");
	else if (token == GREATER_2)
		new_input_split[*i] = ft_strdup(">>");
	*i += 1;
	new_input_split[*i] = ft_strdup(word);
}

/**
 * @brief	Fills the new input split array. It uses fill_token to specify from
 * where the word to assign starts.
 *
 * @param	new_input_split	new input_split
 * @param	input_split		original input_split from ft_split_minishell
 * @param	j				index of the string with no space after a token
 * @param	token 			type of token to be added as a separate str in array
 */
void	fill_new_input_split(char **new_input_split, char **input_split, int j,
	t_tokens token)
{
	int		i;
	int		new_i;

	i = 0;
	new_i = 0;
	while (input_split[i])
	{
		if (i == j)
		{
			if (token == PIPE)
				fill_token(token, new_input_split, &new_i, &input_split[i][1]);
			else if (token == LESS)
				fill_token(token, new_input_split, &new_i, &input_split[i][1]);
			else if (token == LESS_2)
				fill_token(token, new_input_split, &new_i, &input_split[i][2]);
			else if (token == GREATER)
				fill_token(token, new_input_split, &new_i, &input_split[i][1]);
			else if (token == GREATER_2)
				fill_token(token, new_input_split, &new_i, &input_split[i][2]);
		}
		else
			new_input_split[new_i] = ft_strdup(input_split[i]);
		new_i++;
		i++;
	}
}

/**
 * @brief	Handles the reconstruction of the input_split if it contains
 * a string with no space after a token.
 *
 * @param	input_split	original input_split from ft_split_minishell
 * @param	index		index of the string with no space after a token
 * @param	token		type of token present in the string
 * @return	char**		new input_split, with lenght +1 vs original input_split
 */
char	**no_space_split(char **input_split, int index, t_tokens token)
{
	int		i;
	int		len_2d;
	char	**new_input_split;

	i = 0;
	while (input_split[i])
		i++;
	len_2d = i;
	new_input_split = (char **) malloc(sizeof(char *) * (len_2d + 2));
	if (!new_input_split)
		return (NULL);
	fill_new_input_split(new_input_split, input_split, index, token);
	new_input_split[len_2d + 1] = NULL;
	free_array(input_split);
	return (new_input_split);
}
