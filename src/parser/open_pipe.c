/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 21:33:29 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/27 21:39:05 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief	Initializes a temporary data structure.
 * 
 * The tmp_data struct uses the env_list from the original data struct, process
 * additional input via input and input_split variables and has lexer and
 * simple_cmds set to NULL as that is what parsing functions expect.
 * 
 * Ctrl + D and newline (enter) input cases are handled as in bash. Ctrl + C
 * case is not handled as it was deemed out of scope (the handle_open_pipe
 * which is the caller of this function is a nice-to-have).
 * 
 * @param	tmp_data	pointer to a temporary t_data structure 
 * @param	env_list	list of env variables from the original data struct
 */
static void	init_tmp_data(t_data *tmp_data, t_list *env_list)
{
	tmp_data->env_list = env_list;
	tmp_data->lexer = NULL;
	tmp_data->simple_cmds = NULL;
	while (42)
	{
		tmp_data->input = readline("> ");
		if (tmp_data->input == NULL)
		{
			ft_putendl_fd("minishell: syntax error: unexpected end of "
				"file", STDERR);
			ft_putendl_fd("exit", STDOUT);
			exit_minishell(NULL, 2);
		}
		if (tmp_data->input[0] != '\0')
			break ;
	}
	tmp_data->input_split = ft_split_minishell(tmp_data->input, ' ');
}

/**
 * @brief	Handles the case where the last character in the input is a pipe.
 * 
 * This function checks if the last character in the input is a pipe. If it is,
 * it handles the waiting and parsing of additional input provided
 * by the user. It initializes a temporary data structure, converts the input
 * into a lexer list and simple commands, expands the commands, handles heredoc
 * and then adds the additional input (now in the form of simple_cmds nodes)
 * at the end of the simple_cmds list in the original data structure.
 * Then it frees the temporary input, input_split and lexer. The free_data
 * function is not used here as the tmp_data struct wasn't used in its full
 * scope and some data is still needed by the original data struct (env_list and 
 * the newly added simple_cmds nodes as ft_lstadd_back just assigns pointers, 
 * not copies).
 * 
 * @param	data	pointer to the t_data structure 
 */
void	handle_open_pipe(t_data *data)
{
	t_data	tmp_data;
	t_list	*current;
	int		len_2d;

	len_2d = 0;
	while (data->input_split[len_2d])
		len_2d++;
	if (!ft_strncmp(data->input_split[len_2d - 1], "|", 2))
	{
		init_tmp_data(&tmp_data, data->env_list);
		input_arr_to_lexer_list(&tmp_data);
		lexer_to_simple_cmds(&tmp_data.lexer, &tmp_data.simple_cmds);
		expander(&tmp_data);
		heredoc(&tmp_data);
		current = tmp_data.simple_cmds;
		while (current)
		{
			ft_lstadd_back(&data->simple_cmds, current);
			current = current->next;
		}
		free(tmp_data.input);
		free_array(tmp_data.input_split);
		free_lexer(&tmp_data.lexer);
	}
}
