/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 07:40:43 by vbartos           #+#    #+#             */
/*   Updated: 2024/01/24 09:21:27 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief Initializes environment variables into a linked list.
 *
 * This function initializes the environment variables into a linked list by
 * adding each variable from the provided array (envp). It calls the env_add
 * function to create and append new nodes to the linked list, representing
 * each environment variable.
 *
 * @param envp An array of strings representing the initial environment
 * variables.
 * @param data A pointer to the data structure containing the linked list
 * of environment variables.
 * @return 0 on successful initialization, or appropriate exit code on failure.
 */
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

/**
 * @brief Adds a new variable to the minishell's environment variables list.
 *
 * This function adds a new variable to the linked list of environment variables
 * used by minishell. It splits the variable into a name-value pair and handles
 * cases where the variable has no '=' (assigns no value). The new variable is
 * added to the list.
 *
 * @param head A pointer to the pointer to the head of the linked list.
 * @param env_var The string representing the environment variable to be added.
 * @return 0 on success or appropriate exit code on failure.
 */
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
		exit_minishell(NULL, EXIT_MALLOC);
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

/**
 * @brief Finds a variable in the local environment variables list.
 *
 * This function searches for a variable with a specified key in the linked list
 * of environment variables. If found, it returns a pointer to the node
 * containing the variable; otherwise, it returns NULL.
 *
 * @param head A pointer to the head of the linked list.
 * @param variable_key The key of the variable to find.
 * @return A pointer to the node containing the variable, or NULL if not found.
 */
t_list	*env_find(t_list *head, char *variable_key)
{
	t_env	*content;

	if (head == NULL)
		return (NULL);
	content = head->content;
	if (ft_strncmp(content->name, variable_key, ft_strlen(variable_key)) == 0
		&& (ft_strlen(content->name) == ft_strlen(variable_key)))
		return (head);
	return (env_find(head->next, variable_key));
}

/**
 * @brief Removes a variable from the local environment variables list.
 *
 * This function removes a variable with the specified key from the linked list
 * of environment variables. It returns a pointer to the modified head of the
 * linked list.
 *
 * @param head A pointer to the pointer to the head of the linked list.
 * @param variable_key The key of the variable to be removed.
 * @return A pointer to the modified head of the linked list.
 */
t_list	**env_remove(t_list **head, char *variable_key)
{
	t_list	*var_to_remove;
	t_list	*var_before;
	t_list	*var_after;
	t_env	*content;

	var_to_remove = env_find(*head, variable_key);
	if (var_to_remove == NULL)
		return (head);
	if (var_to_remove == *head)
		*head = (*head)->next;
	else
	{
		var_before = *head;
		while (var_before->next != var_to_remove)
			var_before = var_before->next;
		var_after = var_to_remove->next;
		var_before->next = var_after;
	}
	content = (t_env *) var_to_remove->content;
	free(content->full_string);
	free(content->name);
	free(content->value);
	free(var_to_remove->content);
	free(var_to_remove);
	return (head);
}
