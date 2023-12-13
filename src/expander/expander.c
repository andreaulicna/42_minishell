/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:14:28 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/13 22:14:51 by aulicna          ###   ########.fr       */
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
 * @brief	Expands dollar signs in a command string based on specific
 * conditions.
 * 
 * This function iterates through a command string 'content->cmd[i]', determines
 * whether or not to expand a dollar sign based on the predefined conditions
 * represented by the checker_dollar function and does so where appropriate.
 * 
 * @param	content		pointer to t_simple_cmds struct
 * @param	i			index of the command string in the array to process
 * @param	env_list	linked list containing environment variables
 */
void	expander_loop_dollar(t_simple_cmds *content, int i, t_list *env_list)
{
	int	j;
	int	dollar_flag;

	j = 0;
	while (content->cmd[i][j])
	{
		if (content->cmd[i][j] == '$')
		{
			dollar_flag = checker_dollar(content->cmd[i], j);
			delete_quotes(content->cmd, i);
			if (dollar_flag == 5)
				break ;
			else if (dollar_flag == 3)
				delete_backslash(content->cmd, i);
			else if (dollar_flag == 1)
				expand_exit_code(content->cmd, i);
			else if (dollar_flag == 0)
				expand_dollar(content->cmd, i, env_list);
		}
		j++;
	}
}

/**
 * @brief	Handles a string that doesn't contain a dollar signs.
 * 
 * First, quotes from the command string 'content->cmd[i]' are deleted. Then
 * the function iterates through the string and deletes any backslash
 * encountered.
 * 
 * @param	content		pointer to t_simple_cmds struct
 * @param	i			index of the command string in the array to process
 */
void	expander_loop_no_dollar(t_simple_cmds *content, int i)
{
	int	j;

	delete_quotes(content->cmd, i);
	j = 0;
	while (content->cmd[i][j])
	{
		if (content->cmd[i][j] == '\\')
			delete_backslash(content->cmd, i);
		j++;
	}
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
				expander_loop_no_dollar(content, i);
			i++;
		}
		current = current->next;
	}
}
