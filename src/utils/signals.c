/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 14:36:46 by aulicna           #+#    #+#             */
/*   Updated: 2024/02/05 17:31:32 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief	Handles the SIGINT signal.
 *
 * This function is designed to handle the SIGINT signal (pressing Ctrl+C) when
 * received on the minishell prompt input (not in heredoc or hanging command).
 * 
 * When the SIGINT signal is received, the g_signal is set to SIGINT and checked
 * later on in the code, correctly assigning an exit_status of 130 if needed.
 * 
 * Then the function writes a newline character to the standard output, prepares
 * readline library to accept a new line, replaces the current line
 * with an empty line, and then redisplay the prompt.
 *
 * @param	sig_num	signal number
 */
void	handle_sigint(int sig_num)
{
	if (sig_num == SIGINT)
	{
		g_signal = SIGINT;
		write(STDOUT, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_sigint_with_child(int sig_num)
{
	if (sig_num == SIGINT)
		g_signal = SIGINT;
}

/**
 * @brief	Handles the SIGINT and SIGUSR1 signals for a heredoc operation that
 * was interrupted by SIGINT.
 *
 * @param	sig_num	signal number
 */
void	handle_sigint_heredoc(int sig_num)
{
	if (sig_num == SIGINT)
	{
		write(STDOUT, "\n", 1);
		exit_minishell(NULL, 130);
	}
}

void	reset_signals_default(void)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
}

int	signal_exit_of_child(int *status)
{
	if (WTERMSIG(*status) == g_signal)
	{
		write(STDOUT, "\n", 1);
		g_signal = 0;
		return (130);
	}
	if (WTERMSIG(*status) == SIGQUIT)
	{
		write(1, "Quit\n", 5);
		return (131);
	}
	return (0);
}
