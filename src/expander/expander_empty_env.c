/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_empty_env.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 13:01:16 by aulicna           #+#    #+#             */
/*   Updated: 2024/02/02 14:29:43 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief	Duplicates a 2D array of strings.
 * 
 * @param	arr		original 2D array of strings
 * @return	char**	new (duplicate of input) 2D array of strings
 */
char	**ft_strdup_array(char **arr)
{
	int		i;
	int		len_2d;
	char	**new_arr;

	if (arr[0] == NULL)
		return (NULL);
	len_2d = 0;
	while (arr[len_2d])
		len_2d++;
	new_arr = ft_calloc(len_2d + 1, sizeof(char *));
	i = 0;
	while (arr[i])
	{
		new_arr[i] = ft_strdup(arr[i]);
		i++;
	}
	new_arr[i] = NULL;
	return (new_arr);
}

/**
 * @brief	Checks if all strings in a simple_cmds array are empty.
 * 
 * @param	cmd	simple_cmds array to check
 * @return	int	1 if all strings in the command are empty, 0 otherwise
 */
static int	only_empty_envs(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i][0] != '\0')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief	Removes a string from simple_cmds array.
 * 
 * @param	cmd		original simple_cmds array
 * @param	i		index of the string to remove
 * @return	char**	new simple_cmds array that is a copy of the original one,
 * 					but without the string at index i
 */
static char	**remove_cmd_empty_env(char **cmd, int i)
{
	int		len_2d;
	int		j;
	int		new_j;
	char	**new_cmd;

	len_2d = 0;
	while (cmd[len_2d])
		len_2d++;
	new_cmd = ft_calloc(len_2d, sizeof(char *));
	j = 0;
	new_j = 0;
	while (cmd[j])
	{
		if (j == i)
			j++;
		new_cmd[new_j] = ft_strdup(cmd[j]);
		j++;
		new_j++;
	}
	new_cmd[new_j] = NULL;
	free_array(cmd);
	return (new_cmd);
}

/**
 * @brief	Handles empty environment variables in a simple_cmds array.
 * 
 * It traverses through the simple_cmds array and once it encounters an empty
 * string, it checks whether it originally contained a dollar sign. If it did, 
 * it indicates that it used to be a non-existed env variable and should
 * be removed from the array.
 * 
 * @param	old_cmd		original simple_cmds array before expander
 * @param	content		simple_cmds node
 * @param	exit_status	pointer to the most recet exit status
 */
void	handle_empty_envs(char **old_cmd, t_simple_cmds *content,
	int *exit_status)
{
	int	i;
	int	old_i;

	if (only_empty_envs(content->cmd))
	{
		*exit_status = 0;
		free_array(old_cmd);
		exit_current_prompt(NULL);
	}
	i = 0;
	old_i = 0;
	while (content->cmd[i])
	{
		if (content->cmd[i][0] == '\0' && contains_dollar(old_cmd[old_i]))
		{
			content->cmd = remove_cmd_empty_env(content->cmd, i);
			i--;
		}
		i++;
		old_i++;
	}
	free_array(old_cmd);
}
