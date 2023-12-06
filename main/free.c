/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 14:27:08 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/06 14:57:04 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
	if (data->env_list != NULL)
		free_envlist(&data->env_list);
}