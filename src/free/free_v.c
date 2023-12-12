/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_v.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 14:27:08 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/12 11:00:19 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

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