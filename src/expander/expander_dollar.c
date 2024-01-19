/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_dollar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 22:16:33 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/13 16:45:43 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief	Checks the context of the dollar sign in a string.
 * 
 * This function analyzes the context of a dollar sign ('$') in a string 'str'
 * at position 'j'. It examines nearby characters to determine the context
 * of the dollar sign within the string.
 * 
 * @param	str	input string to check
 * @param	j	index of the dollar sign in the string
 * @return	int	returns a flag representing the context:
 *              5:	$ is enclosed in single quotes
 * 				4:	$ is followed by \ and enclosed in double quotes
 * 				3:	$ is followed or preceded with \ (not in double quotes)
 *              2:	$ is followed by a space, a quote (' or ")
 * 					or there is nothing else following it
 *              1:	$ is followed by a question mark (?)
 *              0: 	$ appears in a context that can be expanded
 */
int	checker_dollar(char *str, int j)
{
	if ((str[0] == '\'' && str[ft_strlen_custom(str) - 1] == '\''))
		return (5);
	if (str[0] == '"' && str[ft_strlen_custom(str) - 1] == '"'
		&& str[j + 1] == '\\')
		return (4);
	if (j - 1 >= 0)
	{
		if (str[j - 1] == '\\')
			return (3);
	}
	if (str[j + 1] == '\\')
		return (3);
	if (!str[j + 1] || str[j + 1] == ' ' || str[j + 1] == '\''
		|| str[j + 1] == '"')
		return (2);
	if (str[j + 1] == '?')
		return (1);
	return (0);
}

/**
 * @brief Expands a dollar sign followed by the exit status in a string.
 * 
 * It replaces the exit status placeholder with the exit status of the most
 * recently executed foreground pipeline.
 * 
 * It breaks down the input string into 3 parts:
 * part_1: up until, but not including, the dollar sign
 * part_2: the exit status
 * part_3: after, not including, the question mark that follows the dollar sign
 * 
 * Then it constructs the final string in 2 steps, first joining part_1 and
 * part_2 into tmp_join and then joining tmp_join and part_3 into final.
 * 
 * Finally, the appropriate string in the 2D input array is pointed to the final
 * string. All of the dynamically allocated memory is then freed with the
 * exception of the final string that is not freed until after the whole command
 * is processed and the program reaches the free_simple_cmds function.
 * 
 * @param 	cmd			array of strings containing input commands
 * @param	i_cmd		index of the command string in the array to modify
 * @param	exit_status	exit status of the most recently executed foreground
 * 						pipeline
 */
void	expand_exit_status(char **cmd, int i_cmd, int exit_status)
{
	char	*str;
	int		i;
	int		j;
	t_str	new_str;

	str = cmd[i_cmd];
	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	init_struct_str(&new_str);
	new_str.part_1 = ft_substr(str, 0, i);
	new_str.part_2 = ft_itoa(exit_status);
	j = 2;
	new_str.part_3 = ft_substr(str, i + j, ft_strlen_custom(str) - i - j);
	new_str.tmp_join = ft_strjoin(new_str.part_1, new_str.part_2);
	new_str.final = ft_strjoin(new_str.tmp_join, new_str.part_3);
	cmd[i_cmd] = new_str.final;
	free_struct_str(&new_str, str);
}

/**
 * @brief	This is a helper function to the expand_dollar function that helps
 * with constructing the final string based on the different parts of the input
 * string and the value of the env variable that was found.
 * 
 * If the env variable is found, the join happens in the expand_dollar_found
 * function. Otherwise, the part_2 is skipped in the join and the final str
 * includes only the part_1 and part_3. When there is no env value, j_cmd is
 * reset to -1 to avoid invalid read size error when the control is returned
 * back to the expander_loop_dolar function.
 * 
 * @param	new_str	pointer to a t_str structure containing parsed string parts
 * @param	j_cmd		index of the dollar sign in string to modify
 */
void	expand_dollar_construct_final(t_str *new_str, int *j_cmd)
{
	if (new_str->env_found != NULL)
	{
		new_str->content = (t_env *) new_str->env_found->content;
		new_str->tmp_join = ft_strjoin(new_str->part_1,
				new_str->content->value);
		new_str->final = ft_strjoin(new_str->tmp_join, new_str->part_3);
	}
	else
	{
		new_str->final = ft_strjoin(new_str->part_1, new_str->part_3);
		*j_cmd = -1;
	}
}

/**
 * @brief	Expands a dollar sign followed by an environment variable.
 * 
 * The function identifies the environment variable in the input string and
 * replaces it with its value from the provided environment list.
 * 
 * It breaks down the input string into 3 parts:
 * part_1: up until, but not including, the dollar sign
 * part_2: up until, and including, the first space present in the rest of
 * the string or if there is no space, the end of the string
 * part_3: after the space following the dollar sign, potentially NULL if there
 * is no space in the string after the dollar sign
 * 
 * Then it constructs the final string, which can happen in 2 ways depending on
 * whether or not the env variable was found in the env list. This part is done
 * in the expand_dollar_construct_final helper function.
 * 
 * Finally, the appropriate string in the 2D input array is pointed to the final
 * string. All of the dynamically allocated memory is then freed with the
 * exception of the final string that is not freed until after the whole command
 * is processed and the program reaches the free_simple_cmds function.
 * 
 * @param	cmd			array of strings containing input commands
 * @param	i_cmd		index of the command string in the array to modify
 * @param	env_list	linked list containing environment variables
 * @param	j_cmd		index of the dollar sign in string to modify
 */
void	expand_dollar(char **cmd, int i_cmd, t_list *env_list, int *j_cmd)
{
	char	*str;
	int		i;
	int		j;
	t_str	new_str;

	str = cmd[i_cmd];
	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	init_struct_str(&new_str);
	new_str.part_1 = ft_substr(str, 0, i);
	j = 0;
	while (str[i + j] && str[i + j] != ' ')
		j++;
	new_str.part_2 = ft_substr(str, i + 1, j - 1);
	new_str.part_3 = ft_substr(str, i + j, ft_strlen_custom(str) - i - j);
	new_str.env_found = env_find(env_list, new_str.part_2);
	expand_dollar_construct_final(&new_str, j_cmd);
	cmd[i_cmd] = new_str.final;
	free_struct_str(&new_str, str);
}

/**
 * @brief	Deletes a backslash that follows or precedes a dollar sign ($).
 * 
 * It breaks down the input string into 2 parts, leaving the backslash out,
 * and then it constructs the final string by joining the 2 parts.
 * 
 * Finally, the appropriate string in the 2D input array is pointed to the final
 * string. All of the dynamically allocated memory is then freed with the
 * exception of the final string that is not freed until after the whole command
 * is processed and the program reaches the free_simple_cmds function.
 * 
 * Examples:
 * \$USER -> $USER 
 * $\USER -> $USER 
 * "$\USER" -> $\USER 
 * "\$USER" -> $USER 
 * '$\USER' -> $\USER 
 * '\$USER' -> \$USER 
 * 
 * @param	cmd			array of strings containing input commands
 * @param	i_cmd		index of the command string in the array to modify
 */
void	delete_backslash(char **cmd, int i_cmd)
{
	char	*str;
	int		i;
	t_str	new_str;

	str = cmd[i_cmd];
	i = 0;
	while (str[i] && str[i] != '\\')
		i++;
	init_struct_str(&new_str);
	new_str.part_1 = ft_substr(str, 0, i);
	new_str.part_2 = ft_substr(str, i + 1, ft_strlen(str) - 1);
	new_str.final = ft_strjoin(new_str.part_1, new_str.part_2);
	cmd[i_cmd] = new_str.final;
	free_struct_str(&new_str, str);
}
