/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 14:27:08 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/12 13:17:21 by vbartos          ###   ########.fr       */
=======
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 13:37:39 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/12 13:42:00 by aulicna          ###   ########.fr       */
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

<<<<<<< HEAD
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

=======
>>>>>>> main
// free_data
// - frees the entire data struct;
void	free_data(t_data *data)
{
<<<<<<< HEAD
	if (data->env_list != NULL)
		free_envlist(&data->env_list);
=======
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
>>>>>>> main
}
