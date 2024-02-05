/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:14:28 by aulicna           #+#    #+#             */
/*   Updated: 2024/02/05 01:33:36 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

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
 * @param	exit_status	most recent exit status
 * @param	env_list	list of environment variables
 */
void	expander_loop_dollar(t_simple_cmds *content, int i, int exit_status,
	t_list *env_list)
{
	int	j;
	int	dollar_flag;

	j = 0;
	while (content->cmd[i][j])
	{
		if (content->cmd[i][j] == '$')
		{
			dollar_flag = checker_dollar(content->cmd[i], j);
			while (has_quotes_to_delete(content->cmd[i]))
				content->cmd[i] = delete_quotes(content->cmd[i]);
			if (dollar_flag == 5)
				break ;
			else if (dollar_flag == 3)
				content->cmd[i] = delete_backslash(content->cmd[i]);
			else if (dollar_flag == 1)
				content->cmd[i] = expand_exit_status(content->cmd[i],
						exit_status);
			else if (dollar_flag == 0)
				content->cmd[i] = expand_dollar(content->cmd[i], env_list, &j);
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
static void	expander_loop_no_dollar(t_simple_cmds *content, int i)
{
	int	j;

	while (has_quotes_to_delete(content->cmd[i]))
		content->cmd[i] = delete_quotes(content->cmd[i]);
	j = 0;
	while (content->cmd[i][j])
	{
		if (content->cmd[i][j] == '\\')
			content->cmd[i] = delete_backslash(content->cmd[i]);
		j++;
	}
}

/**
 * @brief	Traverses a word (from the redirect list) and determines whether
 * or not to expand a dollar sign based on the predefined conditions
 * represented by the checker_dollar function and does so where appropriate.
 * 
 * @param	content		content of the current redirect node being processed
 * @param	exit_status	most recent exit status
 * @param	env_list	list of environment variables
 */
static void	expander_redirects_loop_dollar(t_lexer *content, int exit_status,
	t_list *env_list)
{
	int		i;
	int		dollar_flag;

	i = 0;
	while (content->word[i])
	{
		if (content->word[i] == '$')
		{
			dollar_flag = checker_dollar(content->word, i);
			while (has_quotes_to_delete(content->word))
				content->word = delete_quotes(content->word);
			if (dollar_flag == 1)
				content->word = expand_exit_status(content->word,
						exit_status);
			else if (dollar_flag == 0)
				content->word = expand_dollar(content->word,
						env_list, &i);
		}
		i++;
	}
}

/**
 * @brief	Traverses the redirects list (for every node of simple_cmds),
 * either expanding and deleting quotes, or at least deleting quotes if there
 * are any.
 * 
 * The while loop traverses through the nodes of the redirects list
 * ("1st level detail") while the expander_redirects_loop_dollar function
 * traverses through each character of the string saved as the word
 * ("2nd level detail").
 * 
 * @param	redirects	list of redirections
 * @param	data	pointer to the t_data structure (for env_list, exit_status)
 */
static void	expander_redirects(t_list **redirects, t_data *data)
{
	t_list	*current;
	t_lexer	*content;

	current = *redirects;
	while (current)
	{
		content = (t_lexer *) current->content;
		if (contains_dollar(content->word))
			expander_redirects_loop_dollar(content, data->exit_status,
				data->env_list);
		else
		{
			while (has_quotes_to_delete(content->word))
				content->word = delete_quotes(content->word);
		}
		current = current->next;
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
 * Then, expander_redirects is called to remove quotes and expand dollar signs
 * in redirections (word) too.
 * 
 * Lastly, the cmd array is checked for empty strings that are left after
 * expanding env variables that don't exist and they're removed from the array.
 *  
 * @param	data	pointer to the t_data structure (for env_list, exit_status)
 */
void	expander(t_data *data)
{
	t_list			*current;
	t_simple_cmds	*content;
	int				i;
	char			**old_cmd;

	current = data->simple_cmds;
	while (current != NULL)
	{
		i = -1;
		content = (t_simple_cmds *) current->content;
		old_cmd = ft_strdup_array(content->cmd);
		while (content->cmd[++i])
		{
			while (env_in_quotes_followed(content->cmd[i]))
				expander_loop_dollar_no_space(data, content, i);
			if (contains_dollar(content->cmd[i]))
				expander_loop_dollar(content, i, data->exit_status,
					data->env_list);
			else
				expander_loop_no_dollar(content, i);
		}
		expander_redirects(&content->redirects, data);
		handle_empty_envs(old_cmd, content, &data->exit_status);
		current = current->next;
	}
}
