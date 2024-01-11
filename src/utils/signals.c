/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:33:13 by aulicna           #+#    #+#             */
/*   Updated: 2024/01/11 22:07:54 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief Handles the SIGINT signal.
 *
 * This function is called when the SIGINT signal is received.
 * It writes a newline character to the standard output,
 * moves the readline cursor to a new line,
 * replaces the current line with an empty string, and redisplays the prompt.
 *
 * @param signum The signal number (ignored).
 */
void handle_sigint(int signum) 
{
	write(STDOUT, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * @brief Initializes the signal handlers for the program.
 * 
 * This function sets up the signal handlers for SIGINT and SIGQUIT.
 * The SIGINT signal is handled by the handle_sigint function,
 * while the SIGQUIT signal is ignored.
 */
void init_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}