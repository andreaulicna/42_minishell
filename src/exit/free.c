/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 13:37:39 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/12 13:42:00 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

// free_data
// - frees the entire data struct;
void	free_data(t_data *data)
{
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
}
