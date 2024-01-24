/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:07:57 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/24 15:00:49 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

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
	else if (!ft_strncmp(">>", check, len))
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
 * @brief	Handles redirections without spaces by detecting them and
 * reconstructing the input_split accordingly.
 *
 * As long as there is a redirection token not separated by spaces, the function
 * will keep calling the no_space_split that will keep reconstructing the split.
 * 
 * @param	data	pointer to the t_data structure (for input_split)
 */
void	handle_redirect_no_space(t_data *data)
{
	int			i;
	char		**input_split;
	char		q;

	input_split = data->input_split;
	i = 0;
	while (input_split[i])
	{
		if ((get_quotes_type(input_split[i], &q) && (input_split[i][0] != '<'
				&& input_split[i][0] != '>')) || is_token(input_split[i]))
		{
			i++;
			continue ;
		}
		if (ft_strchr(input_split[i], '|') || ft_strchr(input_split[i], '<')
			|| ft_strchr(input_split[i], '>'))
		{
			data->input_split = no_space_split(input_split, i);
			input_split = data->input_split;
		}
		i++;
	}
}

/**
 * @brief	Converts an input string array into a lexer-linked list where each
 * node represents either a token or a word from the input.
 * 
 * At the beginning, it checks for the case of heredoc being passed without
 * space, i.e. '<<EOF', and recreates the input_split to cater for it.
 * 
 * For each node of the lexer list, either the word or the token is filled in.
 * 
 * @param	data	pointer to the t_data structure (for input_split, lexer)
 * @return	int		returns 0 upon successfully creating lexer
 */
int	input_arr_to_lexer_list(t_data *data)
{
	t_list	*current;
	t_lexer	*content;
	int		i;

	handle_redirect_no_space(data);
	current = NULL;
	i = 0;
	while (data->input_split[i])
	{
		content = malloc(sizeof(t_lexer));
		if (!content)
			exit_minishell(NULL, EXIT_MALLOC);
		content->id = i;
		if (is_token(data->input_split[i]))
			assign_token(content, is_token(data->input_split[i]));
		else
			assign_word(content, data->input_split[i]);
		current = ft_lstnew(content);
		ft_lstadd_back(&data->lexer, current);
		i++;
	}
	return (EXIT_SUCCESS);
}
