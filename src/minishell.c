/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:26:36 by vbartos           #+#    #+#             */
/*   Updated: 2023/11/27 12:34:11 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int main(void)
{
	// char	*prompt;
	// char	*cmd;
	// size_t	n;

	// prompt = PROMPT;
	// cmd = NULL;
	// n = 0;
	// while (1)
	// {
	// 	printf("%s", prompt);
	// 	getline(&cmd, &n, stdin);
	// }
	// free(cmd);

	// char *echo_cmd[] = {"echo", "ahojda", "kamarade", NULL};
	char *echo_cmd2[] = {"echo", "-n", "ahojda", "kamarade", NULL};
	ft_echo(echo_cmd2);
	return (0);
}