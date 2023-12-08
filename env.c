/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 07:40:43 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/08 14:19:26 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"

// env_init
// - initialize all environment variables into a linked list;
int	env_init(char **envp, t_data *data)
{
	t_list	*head;

	head = NULL;
	while (*envp != NULL)
	{
		env_add(&head, *envp);
		envp++;
	}
	data->env_list = head;
	return (0);
}

// env_add
// - adds a new variable to minishell's environment variables list;
// - splits the variable into name+value pair;
// - if added variable has no '=', assings no value;
int	env_add(t_list **head, char *env_var)
{
	t_list	*new_var;
	t_env	*env;
	size_t	equal_pos;

	equal_pos = 0;
	while (env_var[equal_pos] != '=' && env_var[equal_pos] != '\0')
		equal_pos++;
	env = malloc(sizeof(t_env));
	if (!env)
		return (1);
	env->full_string = ft_strdup(env_var);
	env->name = ft_substr(env_var, 0, equal_pos);
	if (env_var[equal_pos] == '=')
		env->value = ft_strdup(env_var + equal_pos + 1);
	else
		env->value = NULL;	
	new_var = ft_lstnew(env);
	ft_lstadd_back(head, new_var);
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
	if (var_to_remove == NULL)
		return (head);
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
	free(var_to_remove->content);
	free(var_to_remove);
	return (head);
}