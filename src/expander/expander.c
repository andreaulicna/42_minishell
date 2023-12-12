/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:14:28 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/12 10:59:51 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief 	Checks if a string contains a dollar sign.
 * 
 * @param	str	input string to be checked
 * @return	int	returns 1 if the string contains a dollar sign; 0 otherwise
 */

int	contains_dollar(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief	Expander prepares the simple_cmds for the executor by deleting
 * quotes and expanding dollar signs where appropriate.
 * on command strings.
 * 
 * This function traverses a linked list of command structures and processes
 * each command calling several helper functions.
 * 
 * The while loop traverses through the strings in the simple_cmds array
 * ("1st level detail") while the expander_loop_dollar function traverses
 * through each character of a string ("2nd level detail").
 *  
 * @param	data	pointer to the t_data structure containing
 */

void	expander(t_data *data)
{
	t_list			*current;
	t_simple_cmds	*content;
	int				i;

	current = data->simple_cmds;
	while (current != NULL)
	{
		i = 0;
		content = (t_simple_cmds *) current->content;
		while (content->cmd[i])
		{
			if (contains_dollar(content->cmd[i]))
				expander_loop_dollar(content, i, data->env_list);
			else
				delete_quotes(content->cmd, i);
			i++;
		}
		current = current->next;
	}
}
