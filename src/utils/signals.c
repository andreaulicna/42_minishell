/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 14:36:46 by aulicna           #+#    #+#             */
/*   Updated: 2024/02/06 13:48:57 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief	Primary SIGINT signal handler used by the parent process.
 *
 * This function handles the SIGINT signal (pressing Ctrl+C) when minishell is
 * in interactive mode, meaning that it is waiting for user input.
 * 
 * When the SIGINT signal is received, the g_signal is set to SIGUSR1 and
 * checked later on in the code to correctly assign an exit status of 130.
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
		g_signal = SIGUSR1;
		write(STDOUT, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

/**
 * @brief	 Secondary SIGINT signal handler used by the parent process.
 * 
 * This function handles the SIGINT signal (pressing Ctrl+C) when minishell
 * is in noninteractive mode, meaning it is not waiting for user input, but
 * rather having a child process run (a, possibly hanging, command, heredoc).
 * 
 * When the SIGINT signal is received, the g_signal is set to SIGINT so that it
 * can be used to check whether a child process exited due to that signal and
 * the parent process can behave accordingly
 * (as defined in signal_exit_of_child).
 * 
 * @param	sig_num	signal number
*/
void	handle_sigint_when_child_running(int sig_num)
{
	if (sig_num == SIGINT)
		g_signal = SIGINT;
}

/**
 * @brief	SIGINT signal handler for a heredoc process used by the child
 * process.
 * 
 * This function handles the SIGINT signal (pressing Ctrl+C) when minishell is
 * processing a heredoc.
 * 
 * When the SIGINT signal is received, the child process prints a newline and
 * exits with an exit status of 130.
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

/**
 * @brief	Signals helper function that resents the SIGQUIT and SIGINT signals
 * to their default behavior used by the child process.
 * 
 * This function is called every time a new child process is created
 * (in exec, not heredoc) to reset the signal handlers that the child process
 * inherits from the parent process.
 * 
 * The rationale is that it should be only the parent process handling
 * the signals as defined in the handlers while the child process should
 * behave 'in a normal way'. When the child process exits, the parent process
 * can check whether it exited because of one of these signals and behave
 * accordingly.
*/
void	reset_signals_default(void)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
}

/**
 * @brief	Signals helper function that defines the behavior of the parent
 * process when a child process (in exec, not heredoc) exits due to a signal.
 * 
 * If the child process exits with the signal that is set in g_signal (SIGINT),
 * the parent process prints a newline, resets g_signal and returns 130 to set
 * the exit status to that value. This scenario covers a hanging command getting
 * interrupted by SIGINT (Ctrl+C).
 * 
 * If the child process exits with the SIGQUIT signal, the parent process prints
 * and a quit message followed by a newline and returns 131 to set the exit
 * status to that value. This scenario covers a hanging command getting
 * interrupted by SIGQUIT (Ctrl+\).
 * 
 * @param	status	exit status of the child process
*/
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
		write(1, "Quit (core dumped)\n", 19);
		return (131);
	}
	return (0);
}
