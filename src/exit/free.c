/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 13:37:39 by aulicna           #+#    #+#             */
/*   Updated: 2024/02/02 14:50:35 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief	Sets variables that are being malloced and then checked if they
 * exist before calling free on them to NULL.
 * 
 * @param	data	pointer to the t_data structure (for input_split, lexer,
 * 					simple_cmds, and pid_list)
*/
static void	set_to_null(t_data *data)
{
	data->input_split = NULL;
	data->lexer = NULL;
	data->simple_cmds = NULL;
	data->pid_list = NULL;
}

/**
 * @brief	Frees the entire data structure before minishell closes.
 * 
 * @param	data	pointer to the t_data structu
*/
void	free_data(t_data *data)
{
	if (data->env_list)
		free_envlist(&data->env_list);
	if (data->prompt)
		free(data->prompt);
	if (data->input)
		free(data->input);
	if (data->input_split)
		free_array(data->input_split);
	if (data->lexer)
		free_lexer(&data->lexer);
	if (data->simple_cmds)
		free_simple_cmds(&data->simple_cmds);
	if (data->orig_fdin)
		close(data->orig_fdin);
	if (data->orig_fdout)
		close(data->orig_fdout);
	if (data->hd_fd)
		close(data->hd_fd);
	if (data->pid_list)
		free(data->pid_list);
	set_to_null(data);
}

/**
 * @brief	Frees the variables stored in the data structure which are needed
 * to process one input (not neccessarily one line, e.g. heredoc).
 * 
 * @param	data	pointer to the t_data structu
*/
void	free_data_current_prompt(t_data *data)
{
	if (data->prompt)
		free(data->prompt);
	if (data->input_split)
		free_array(data->input_split);
	if (data->lexer)
		free_lexer(&data->lexer);
	if (data->simple_cmds)
		free_simple_cmds(&data->simple_cmds);
	if (data->pid_list)
		free(data->pid_list);
	set_to_null(data);
}
