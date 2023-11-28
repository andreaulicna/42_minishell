/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 07:40:43 by vbartos           #+#    #+#             */
/*   Updated: 2023/11/28 13:27:11 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// env_init
// - initialize all environment variables into a linked list;
int	env_init(char **envp, t_data *data)
{
	t_list	*head;
	t_list	*current;

	head = NULL;
	current = NULL;
	while (*envp != NULL)
	{
		current = ft_lstnew(*envp);
		ft_lstadd_back(&head, current);
		envp++;
	}
	data->env_list = head;
	return (0);
}

// env_add
// - adds a new variable to the local environment variables list;
int	env_add(t_list *head, char *env_var)
{
	t_list	*new_var;
	
	new_var = ft_lstnew(env_var);
	ft_lstadd_back(&head, new_var);
	return (0);
}

// env_find
// - finds a variable in the local environment variables list;
t_list	*env_find(t_list *head, char *variable_key)
{
	if (head == NULL)
		return (NULL);
	if (ft_strncmp(head->content, variable_key, ft_strlen(variable_key)) == 0)
		return (head);
	return (env_find(head->next, variable_key));
}

// env_remove
// - removes a variable from the local envrionment variables list;
t_list	**env_remove(t_list **head, char *variable_key)
{
	t_list *var_to_remove;
	t_list *var_before;
	t_list *var_after;
	
	var_to_remove = env_find(*head, variable_key);
	if (var_to_remove == *head)
	{
		*head = (*head)->next;
	}
	else
	{
		var_before = *head;
		while (var_before->next != var_to_remove)
			var_before = var_before->next;
		var_after = var_to_remove->next;
		var_before->next = var_after;
	}
	free(var_to_remove);
	return (head);
}
