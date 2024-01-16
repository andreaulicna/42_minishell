/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:33:13 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/16 18:00:21 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	handle_sigint(int sig_num)
{
	if (sig_num == SIGINT)
	{
		write(STDOUT, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_sigint_heredoc(int sig_num)
{
	if (sig_num == SIGUSR1)
	{
		write(STDOUT, "\n", 1);
		global_signal = SIGUSR1;
	}
	if (sig_num == SIGINT)
	{
		kill(0, SIGUSR1);
		exit_minishell(NULL, 130);
	}
}
