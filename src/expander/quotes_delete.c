/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_delete.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 12:35:35 by aulicna           #+#    #+#             */
/*   Updated: 2024/02/06 13:46:21 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief	Checks if a string contains quotes and determines the quote type. 
 * If quotes are found, it updates the 'q' parameter with the type encountered.
 * 
 * @param	str	string to be checked for quotes
 * @param	q	pointer to a character variable that stores the quote type found
 * @return	int	returns 1 if quotes are found; 0 otherwise
 */
int	get_quotes_type(char *str, char *q)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\'' && str[i] != '"')
		i++;
	if (str[i])
	{
		*q = str[i];
		return (1);
	}
	else
		return (0);
}

/**
 * @brief	Checks whether there is a pair of quotes to delete from a string.
 * 
 * For commands, it works with a copy of the original command before any
 * expansion was performed to determine when to stop deleting quotes (e.g.
 * one type of quotes enclosed in the other).
 * 
 * It is done by comparing the pointer to their first and last occurence. If
 * those two equal, it means that there are no more quotes to delete as they
 * both evaluate to NULL.
 * 
 * @param	str	string to check
*/
int	has_quotes_to_delete(char *str, char *old_cmd)
{
	if (old_cmd)
	{
		if (old_cmd[0] == '\''
			&& old_cmd[ft_strlen_custom(old_cmd) - 1] == '\'')
			return (0);
	}
	if (ft_strchr(str, '"') != ft_strrchr(str, '"')
		|| ft_strchr(str, '\'') != ft_strrchr(str, '\''))
		return (1);
	return (0);
}

/**
 * @brief	Deletes the outermost quotes from a string.
 * 
 * The functions first checks whether there are some quotes to remove using 
 * the has_quotes function. 
 * 
 * Then it breaks down the input string into 3 parts:
 * part_1: up until, but not including, the first occurrence of the outermost
 * pair of quotes
 * part_2: up until, but not including, the second occurrence of the outermost
 * part of quotes, i.e. in between the outermost pair of quotes
 * part_3: after the second occurrence of the outermost pair of quotes
 * 
 * And then it constructs the final string in 2 steps, first joining part_1 and
 * part_2 into tmp_join and then joining tmp_join and part_3 into final.
 * 
 * All of the dynamically allocated memory is then freed with the exception
 * of the final string that is returned. The final string is not freed until
 * after the whole command is processed and the program reaches
 * the free_simple_cmds function.
 * 
 * @param 	str		string to check and potentially remove the quotes from
 */
char	*delete_quotes(char *str)
{
	int		i;
	int		j;
	char	q;
	t_str	new_str;

	if (get_quotes_type(str, &q))
	{
		i = 0;
		while (str[i] != q)
			i++;
		init_struct_str(&new_str);
		new_str.part_1 = ft_substr(str, 0, i);
		j = 0;
		i++;
		while (str[i + j] != q)
			j++;
		new_str.part_2 = ft_substr(str, i, j);
		new_str.part_3 = ft_substr(str, i + j + 1,
				ft_strlen_custom(str) - i - j);
		new_str.tmp_join = ft_strjoin(new_str.part_1, new_str.part_2);
		new_str.final = ft_strjoin(new_str.tmp_join, new_str.part_3);
		free_struct_str(&new_str, str);
		return (new_str.final);
	}
	return (str);
}

/**
 * @brief	Helper function for the expander_loop_dollar function
 * that manages recursive quotes deletion from a word.
 * 
 * @param	content		pointer to t_simple_cmds struct
 * @param	i			index of the command string in the array to process
*/
void	handle_quotes_deletion(t_simple_cmds *content, int i)
{
	char	*old_cmd;

	old_cmd = ft_strdup(content->cmd[i]);
	content->cmd[i] = delete_quotes(content->cmd[i]);
	while (has_quotes_to_delete(content->cmd[i], old_cmd))
	{
		old_cmd = delete_quotes(old_cmd);
		content->cmd[i] = delete_quotes(content->cmd[i]);
	}
	free(old_cmd);
}
