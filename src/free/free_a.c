/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_a.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 14:27:08 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/12 11:55:11 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	free_lexer(t_list **lexer)
{
	t_list	*tmp;
	t_lexer	*content;

	if (!*lexer)
		return ;
	while (*lexer)
	{
		tmp = (*lexer)->next;
		content = (t_lexer *)(*lexer)->content;
		if (content->word)
			free(content->word);
		free(content);
		free(*lexer);
		*lexer = tmp;
	}
	lexer = NULL;
}

void	free_simple_cmds(t_list **simple_cmds)
{
	t_simple_cmds	*content;
	t_list			*free_redirects;
	t_list			*tmp;

	if (!simple_cmds)
		return ;
	while (*simple_cmds)
	{
		tmp = (*simple_cmds)->next;
		content = (t_simple_cmds *)(*simple_cmds)->content;
		free_redirects = content->redirects;
		free_lexer(&free_redirects);
		free_array(content->cmd);
		free(content);
		free(*simple_cmds);
		*simple_cmds = tmp;
	}
	simple_cmds = NULL;
}

/**
 * @brief 	Frees memory of the dynamically allocated variables
 * in the t_str structure.
 * 
 * The t_str structure is used when a new string needs to be constructed based
 * on the expander rules relating to the dollar sign, $, symbol.
 * 
 * @param	str	pointer to the t_str structure to be freed
 */

void	free_struct_str(t_str *str)
{
	if (str->part_1)
		free(str->part_1);
	if (str->part_2)
		free(str->part_2);
	if (str->part_3)
		free(str->part_3);
	if (str->tmp_join)
		free(str->tmp_join);
}
