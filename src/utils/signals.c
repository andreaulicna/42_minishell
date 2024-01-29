/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:33:13 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/29 13:02:15y aulicna          ###   ########.fr       */
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
 * later on in the code, correctly assigning an exit_status of 130 is needed.
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

/**
 * @brief	Handles the SIGINT and SIGUSR1 signals for a heredoc operation that
 * was interrupted by SIGINT.
 *
 * When the SIGUSR1 signal is received, a newline character is printed
 * to the standard output and the g_signal set to SIGUSR1.
 * 
 * When the SIGINT signal is received, the SIGUSR1 signal is sent to
 * all processes in the current process group and minishell exits.
 *
 * @param	sig_num	The signal number. This function handles SIGUSR1 and SIGINT.
 */
void	handle_sigint_heredoc(int sig_num)
{
	if (sig_num == SIGUSR1)
	{
		g_signal = SIGUSR1;
		write(STDOUT, "\n", 1);
	}
	if (sig_num == SIGINT)
	{
		kill(0, SIGUSR1);
		exit_minishell(NULL, 130);
	}
}

/**
 * @brief	Handles the SIGINT and SIGUSR2 signals for a hanging command that 
 * was interrupted by SIGINT.
 *
 * When the SIGUSR2 signal is received, minishell exits with exit_status 130.
 * It is the child process that listens to this signal.
 * 
 * When the SIGINT signal is received, a new line character is printed 
 * to the standard input, g_signal is set to SIGUSR2 and SIGUSR2 signal is sent
 * to all processes in the current process group and then exits the minishell.
 * g_signal is checked for being equal to SIGUSR2 in the wait_for_pipeline
 * function as it indicates that there was a hanging command (in the child
 * process) and hence the whole command should exit with 130.
 *
 * @param	sig_num	The signal number. This function handles SIGUSR1 and SIGINT.
 */
void	handle_sigint_hanging_command(int sig_num)
{
	if (sig_num == SIGUSR2)
	{
		exit_minishell(NULL, 130);
	}
	if (sig_num == SIGINT)
	{
		write(STDOUT, "\n", 1);
		g_signal = SIGUSR2;
		kill(0, SIGUSR2);
	}
}
