/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 11:03:25 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/19 13:37:29 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * WARNING: NEEDS UPDATING LATER
 * 
 * @brief 	Handles syntax error according to the error code sent.
 * 
 * @param	code	integer representing the error code
 * 					1: nothing following a redirection (or a pipe?)
 * @return	int		returns the exit status code (EXIT_FAILURE)
 */
int	error_handler(int code)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (code == 1)
		ft_putstr_fd("syntax error near unexpected token `newline'\n",
			STDERR_FILENO);
	exit_current_prompt(NULL);
	return (1);
}

/**
* @brief	Handles syntax error when encountering an unexpected double token.
* 
* @param	token	token from the defined enum representing different symbols
* @return	int		returns the exit status code (EXIT_FAILURE)
*/
int	error_parser_double_token(t_tokens token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	if (token == PIPE)
		ft_putstr_fd("|", STDERR_FILENO);
	else if (token == LESS)
		ft_putstr_fd("<", STDERR_FILENO);
	else if (token == LESS_2)
		ft_putstr_fd("<<", STDERR_FILENO);
	else if (token == GREATER)
		ft_putstr_fd(">", STDERR_FILENO);
	else if (token == GREATER_2)
		ft_putstr_fd(">>", STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	exit_current_prompt(NULL);
	return (1);
}
