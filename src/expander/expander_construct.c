/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_construct.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 22:16:33 by aulicna           #+#    #+#             */
/*   Updated: 2024/02/06 13:49:27 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

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
 * Just before the part_2 is created, g_signal is checked and if it equals to
 * SIGINT, exit_status is set to 130 since g_signal == SIGINT indicates that 
 * the most recent new prompt was created after Ctrl + C.
 * 
 * The function then constructs the final string in 2 steps, first joining
 * part_1 and part_2 into tmp_join and then joining tmp_join and part_3 into
 * final.
 * 
 * All of the dynamically allocated memory is then freed with the exception
 * of the final string that is not freed until after the whole command
 * is processed and the program reaches the free_simple_cmds function.
 * 
 * Finally, the new_string.final is returned and assigned to the appropriate
 * string in the 2D input array in the caller function.
 * 
 * Signal management:
 * g_signal set to SIGUSR1 indicates that the previous user input was
 * interrupted by SIGINT (Ctrl+C) and hence the exit status to expand should be
 * 130. Then g_signal is reset, so that the current command (e.g. echo $?)
 * makes it to the execution (that doesn't happen unless g_signal is 0).
 * 
 * @param 	str			string to be expanded
 * @param	exit_status	exit status of the most recently executed foreground
 * 						pipeline
 */
char	*expand_exit_status(char *str, int exit_status)
{
	int		i;
	int		j;
	t_str	new_str;

	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	init_struct_str(&new_str);
	new_str.part_1 = ft_substr(str, 0, i);
	if (g_signal == SIGUSR1)
	{
		exit_status = 130;
		g_signal = 0;
	}
	new_str.part_2 = ft_itoa(exit_status);
	j = 2;
	new_str.part_3 = ft_substr(str, i + j, ft_strlen_custom(str) - i - j);
	new_str.tmp_join = ft_strjoin(new_str.part_1, new_str.part_2);
	new_str.final = ft_strjoin(new_str.tmp_join, new_str.part_3);
	free_struct_str(&new_str, str);
	return (new_str.final);
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
 * @return	int		indicates whether to substract 1 from the iteration
 * index (= yes if env not found) when called from expand_dollar
 */
static int	expand_dollar_construct_final(t_str *new_str)
{
	if (new_str->env_found != NULL)
	{
		new_str->content = (t_env *) new_str->env_found->content;
		new_str->tmp_join = ft_strjoin(new_str->part_1,
				new_str->content->value);
		new_str->final = ft_strjoin(new_str->tmp_join, new_str->part_3);
		return (0);
	}
	else
	{
		new_str->final = ft_strjoin(new_str->part_1, new_str->part_3);
		return (1);
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
 * All of the dynamically allocated memory is then freed with the exception
 * of the final string that is not freed until after the whole command
 * is processed and the program reaches the free_simple_cmds function.
 * 
 * Finally, the new_string.final is returned and assigned to the appropriate
 * string in the 2D input array in the caller function.
 * 
 * @param 	str			string to be expanded
 * @param	env_list	linked list containing environment variables
 * @param	j_cmd		index of the dollar sign in string to modify
 */
char	*expand_dollar(char *str, t_list *env_list, int *j_cmd)
{
	int		i;
	int		j;
	t_str	new_str;

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
	if (expand_dollar_construct_final(&new_str))
		*j_cmd -= 1;
	free_struct_str(&new_str, str);
	return (new_str.final);
}

/**
 * @brief	Expands a dollar sign followed by an environment variable when its
 * enclosed in double quotes and not separate by spaces.
 * 
 * The function identifies the environment variable in the input string and
 * replaces it with its value from the provided environment list.
 * 
 * It breaks down the input string into 3 parts:
 * part_1: up until, but not including, the first quote and the dollar sign
 * part_2: up until, and not including, the second quote
 * part_3: after the second quote, potentially NULL if there is end of string
 * 
 * Then it constructs the final string, which can happen in 2 ways depending on
 * whether or not the env variable was found in the env list. This part is done
 * in the expand_dollar_construct_final helper function.
 * 
 * All of the dynamically allocated memory is then freed with the exception
 * of the final string that is not freed until after the whole command
 * is processed and the program reaches the free_simple_cmds function.
 * 
 * Finally, the new_string.final is assigned to the appropriate string
 * in the 2D input array.
 * 
 * @param	data	pointer to the t_data structure (for env_list)
 * @param	content	simple_cmds node to check
 * @param	j_cmd	index of the cmd with dollar sign
 */
void	expander_loop_dollar_no_space(t_data *data, t_simple_cmds *content,
	int i)
{
	int		j;
	int		k;
	t_str	new_str;

	j = 0;
	while (content->cmd[i][j])
	{
		if (content->cmd[i][j] == '"' && content->cmd[i][j + 1] == '$')
			break ;
		j++;
	}
	init_struct_str(&new_str);
	new_str.part_1 = ft_substr(content->cmd[i], 0, j);
	j += 2;
	k = 0;
	while (content->cmd[i][j + k] && content->cmd[i][j + k] != '"')
		k++;
	new_str.part_2 = ft_substr(content->cmd[i], j, k);
	new_str.part_3 = ft_substr(content->cmd[i], j + k + 1,
			ft_strlen_custom(content->cmd[i]) - j - k - 1);
	new_str.env_found = env_find(data->env_list, new_str.part_2);
	expand_dollar_construct_final(&new_str);
	free_struct_str(&new_str, content->cmd[i]);
	content->cmd[i] = new_str.final;
}

/**
 * @brief	Deletes a backslash that follows or precedes a dollar sign ($).
 * 
 * It breaks down the input string into 2 parts, leaving the backslash out,
 * and then it constructs the final string by joining the 2 parts.
 * 
 * All of the dynamically allocated memory is then freed with the exception
 * of the final string that is not freed until after the whole command
 * is processed and the program reaches the free_simple_cmds function.
 * 
 * Finally, the new_string.final is returned and assigned to the appropriate
 * string in the 2D input array in the caller function.
 * 
 * Examples:
 * \$USER -> $USER 
 * $\USER -> $USER 
 * "$\USER" -> $\USER 
 * "\$USER" -> $USER 
 * '$\USER' -> $\USER 
 * '\$USER' -> \$USER 
 * 
 * @param 	str			string to be expanded
 */
char	*delete_backslash(char *str)
{
	int		i;
	t_str	new_str;

	i = 0;
	while (str[i] && str[i] != '\\')
		i++;
	init_struct_str(&new_str);
	new_str.part_1 = ft_substr(str, 0, i);
	new_str.part_2 = ft_substr(str, i + 1, ft_strlen(str) - 1);
	new_str.final = ft_strjoin(new_str.part_1, new_str.part_2);
	free_struct_str(&new_str, str);
	return (new_str.final);
}
