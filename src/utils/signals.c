/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:33:13 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/17 12:43:27 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"
/**
 * @brief	Handles the SIGINT signal.
 *
 * This function is designed to handle the SIGINT signal (pressing Ctrl+C) when
 * received on the minishell prompt input (not in heredoc).
 * 
 * When the SIGINT signal is received, it writes a newline character
 * to the standard output, prepares readline library to accept a new line,
 * replaces the current line with an empty line, and then redisplay the prompt.
 *
 * @param	sig_num	signal number
 */
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

/**
 * @brief	Handles the SIGINT and SIGUSR1 signals during a heredoc operation.
 *
 * When the SIGUSR1 signal is received, it writes a newline character
 * to the standard output and sets the g_signal to SIGUSR1.
 * 
 * When the SIGINT signal is received, it sends the SIGUSR1 signal to
 * all processes in the current process group and then exits the minishell.
 *
 * @param	sig_num	The signal number. This function handles SIGUSR1 and SIGINT.
 */
void	handle_sigint_heredoc(int sig_num)
{
	if (sig_num == SIGUSR1)
	{
		write(STDOUT, "\n", 1);
		g_signal = SIGUSR1;
	}
	if (sig_num == SIGINT)
	{
		kill(0, SIGUSR1);
		exit_minishell(NULL, 130);
	}
}
