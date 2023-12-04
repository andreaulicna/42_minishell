/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:14:28 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/04 14:26:22 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"

void expander(t_list **simple_cmds)
{
	t_list	*current;
	t_simple_cmds	*content;

	current = *simple_cmds;
	content = (t_simple_cmds *) current->content;
	while(current)
	{

	}
	
}