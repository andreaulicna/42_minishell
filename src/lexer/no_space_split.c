/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_space_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 17:26:37 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/23 16:31:20 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	handle_token_in_middle(char **input_split, int i,
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

void	reconstruct_string(char **input_split, int i,
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
