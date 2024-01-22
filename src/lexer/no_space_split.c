/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_space_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 17:26:37 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/22 17:09:28 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief	Determines the type of token present at the beginning of a string
 * without spaces.
 *
 * @param	s	string to check
 * @return	t_tokens	corresponding token type (t_tokens enum) or 0
 */
t_tokens	contains_token_with_no_space(char *s)
{
	int	len;

	len = ft_strlen_custom(s);
	printf("token no space for: %s\n", s);
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
	else if (s[len - 1] == '|' && s[len] == '\0')
		return (PIPE);
	else if (s[len - 2] == '<' && s[len - 1] == '<' && s[len] == '\0')
		return (LESS_2);
	else if (s[len - 1] == '<' && s[len] == '\0')
		return (LESS);
	else if (s[len - 2] == '>' && s[len - 1] == '>' && s[len] == '\0')
		return (GREATER_2);
	else if (s[len - 1] == '>' && s[len] == '\0')
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
	int		k;
	int		l;
	int		new_i;

	i = 0;
	new_i = 0;
	while (input_split[i])
	{
		if (i == j)
		{
			if (input_split[i][0] == '|' || input_split[i][0] == '<'
				|| input_split[i][0] == '>')
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
			{
				k = 0;
				while (input_split[i][k] != '|' && input_split[i][k] != '<'
					&& input_split[i][k] != '>' && input_split[i][k])
					k++;
				new_input_split[new_i] = (char *) malloc(sizeof(char) * (k + 1));
				l = 0;
				while (l < k)
				{
					new_input_split[new_i][l] = input_split[i][l];
					l++;
				}
				new_input_split[new_i][l] = '\0';
			//	new_i++;
			//	if (token == PIPE)
			//		new_input_split[new_i] = ft_strdup("|");
			//	else if (token == LESS_2)
			//		new_input_split[new_i] = ft_strdup("<<");
			//	else if (token == LESS)
			//		new_input_split[new_i] = ft_strdup("<");
			//	else if (token == GREATER)
			//		new_input_split[new_i] = ft_strdup(">");
			//	else if (token == GREATER_2)
			//		new_input_split[new_i] = ft_strdup(">>");
			}
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
	printf("***NEW SPLIT***\n");
	print_input_split(new_input_split);
	return (new_input_split);
}
