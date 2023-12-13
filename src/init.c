/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 12:06:21 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/13 18:40:45 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	init_data(t_data *data)
{
	data->env_list = NULL;
	data->lexer = NULL;
	data->simple_cmds = NULL;
	data->prompt = NULL;
	data->input = NULL;
	data->input_split = NULL;
	exit_minishell(data, 0);
}

/**
 * @brief 	Initializes the t_str structure with NULL values. 
 * 
 * The main purpose is to avoid conditional jumps based on an uninitialized
 * variable and allow for an easy way of checking whether a particular variable
 * in the struct needs to be freed.
 * 
 * @param	str	pointer to the t_str structure to be initialized
 */

void	init_struct_str(t_str *str)
{
	str->part_1 = NULL;
	str->part_2 = NULL;
	str->part_3 = NULL;
	str->tmp_join = NULL;
	str->final = NULL;
	str->env_found = NULL;
	str->content = NULL;
}
