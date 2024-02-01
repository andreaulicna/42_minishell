/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_space_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 17:26:37 by aulicna           #+#    #+#             */
/*   Updated: 2024/02/01 16:16:25 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief Handles a token that is in the middle of a string that needs to be
 * reconstructed.
 * 
 * This function finds the first occurrence of '<', '>', or '|' in the string
 * at index `i` in `input_split`.
 * It then splits the string into two parts at this character:
 * 1. the part before the character
 * 2. and the character itself with the rest of the string
 * 
 * The part before the character is stored in new_input_split[new_i], and
 * the character with the rest of the string is stored
 * in new_input_split[new_i + 1].
 * 
 * @param	input_split		original input split
 * @param	i				current index in the input_split array
 * @param	new_input_split new input split
 * @param	new_i			current index in the new_input_split array
 */
static void	handle_token_in_middle(char **input_split, int i,
	char **new_input_split, int *new_i)
{
	int	k;
	int	j;

	k = 0;
	while (input_split[i][k] != '<' && input_split[i][k] != '>'
		&& input_split[i][k] != '|')
		k++;
	new_input_split[*new_i] = (char *) malloc(sizeof(char) + (k + 1));
	j = 0;
	while (j < k)
	{
		new_input_split[*new_i][j] = input_split[i][j];
		j++;
	}
	new_input_split[*new_i][j] = '\0';
	*new_i += 1;
	new_input_split[*new_i] = ft_strdup(&input_split[i][k]);
}

/**
 * @brief	Reconstructs a string from the input_split array and stores it
 * in the new_input_split array.
 * 
 * If the token is found at the beginning of the string, it is stored in 
 * new_input_split[new_i] and the rest of the string is stored in
 * new_input_split[new_i + 1].
 * 
 * Else the token is found not at the beginning of the string,
 * handle_token_in_middle is called.
 * 
 * @param	input_split		original input split
 * @param	i				current index in the input_split array
 * @param	new_input_split new input split
 * @param	new_i			current index in the new_input_split array
 */
static void	reconstruct_string(char **input_split, int i,
	char **new_input_split,	int *new_i)
{
	if ((input_split[i][0] == '<' && input_split[i][1] == '<')
		|| (input_split[i][0] == '>' && input_split[i][1] == '>'))
	{
		if (input_split[i][0] == '<')
			new_input_split[*new_i] = ft_strdup("<<");
		else
			new_input_split[*new_i] = ft_strdup(">>");
		*new_i += 1;
		new_input_split[*new_i] = ft_strdup(&input_split[i][2]);
	}
	else if (input_split[i][0] == '|' || input_split[i][0] == '<'
		|| input_split[i][0] == '>')
	{
		if (input_split[i][0] == '|')
			new_input_split[*new_i] = ft_strdup("|");
		else if (input_split[i][0] == '<')
			new_input_split[*new_i] = ft_strdup("<");
		else
			new_input_split[*new_i] = ft_strdup(">");
		*new_i += 1;
		new_input_split[*new_i] = ft_strdup(&input_split[i][1]);
	}
	else
		handle_token_in_middle(input_split, i, new_input_split, new_i);
}

/**
 * @brief	Handles reconstruction of the input_split if it contains a string
 * with a redirection token not separated by spaces.
 * 
 * It reconstructs (splits) only one string, identified by the index.
 * The rest is just copied over.
 * 
 * @param	input_split	original input split
 * @param	i			index of the string that will be reconstructed 
*/
char	**no_space_split(char **input_split, int index)
{
	char	**new_input_split;
	int		len_2d;
	int		i;
	int		new_i;

	len_2d = 0;
	while (input_split[len_2d])
		len_2d++;
	new_input_split = (char **) malloc(sizeof(char *) * (len_2d + 2));
	i = 0;
	new_i = 0;
	while (input_split[i])
	{
		if (i == index)
			reconstruct_string(input_split, i, new_input_split, &new_i);
		else
			new_input_split[new_i] = ft_strdup(input_split[i]);
		new_i++;
		i++;
	}
	new_input_split[len_2d + 1] = NULL;
	free_array(input_split);
	return (new_input_split);
}
