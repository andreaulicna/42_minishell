/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 14:27:08 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/11 11:28:51 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"

// free_array
// - frees a dynamically allocated 2D array;
int	free_array(char **strs)
{
	int	i;

	i = 0;
	while (strs[i] != NULL)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
	return (0);
}

int	free_envlist(t_list **head)
{
	t_list	*current;
	t_list	*next;
	t_env	*env;

	current = *head;
	while (current != NULL) 
	{
		next = current->next;
		env = (t_env *) current->content;
		free(env->full_string);
		free(env->name);
		free(env->value);
		free(current->content);
		free(current);
		current = next;
	}
	*head = NULL;
	return (0);
}

// free_env
// - called as an argument to ft_lstclear;
// - frees the content of the environment variables linked list;
// void	free_env(void *ptr)
// {
// 	t_list	*env;

// 	env = ptr;
// 	if (env->content != NULL)
// 		free(env->content);
// 	free(env);
// 	env = NULL;
// }

void	free_lexer(t_list **lexer)
{
	t_list	*tmp;
	t_lexer	*content;

	if (!*lexer)
		return ;
	while (*lexer)
	{
		tmp = (*lexer)->next;
		content = (t_lexer *) (*lexer)->content;
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
	t_simple_cmds *content;
	t_list	*free_redirects;
	t_list	*tmp;

	if (!simple_cmds)
		return ;
	while (*simple_cmds)
	{
		tmp = (*simple_cmds)->next;
		content = (t_simple_cmds *) (*simple_cmds)->content;
		free_redirects = content->redirects;
		free_lexer(&free_redirects);
		free_array(content->cmd);
		free(content);
		free(*simple_cmds);
		*simple_cmds = tmp;
	}
	simple_cmds = NULL;
}

// free_data
// - frees the entire data struct;
void	free_data(t_data *data)
{
	if (data->env_list)
		free_envlist(&data->env_list);
	if (data->lexer)
		free_lexer(&data->lexer);
	if (data->simple_cmds)
		free_simple_cmds(&data->simple_cmds);
	if (data->input)
		free(data->input);
	if (data->prompt)
		free(data->prompt);
	if (data->input_split)
	{
		free_array(data->input_split);
		data->input_split = NULL;
	}
}

void	exit_minishell(t_data *data, int exit_status)
{
	static t_data	*static_data;

	if (!static_data)
		static_data = data;
	else
	{
		free_data(static_data);
		exit(exit_status);
	}
}