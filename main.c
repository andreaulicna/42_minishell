/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:33:13 by aulicna           #+#    #+#             */
/*   Updated: 2024/02/01 15:56:08 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"

int	g_signal = 0;

/**
 * @brief	Runs the minishell.
 * 
 * Signal lines:
 * 1. SIGINT: Sets up the signal handler for SIGINT (Ctrl + C), so that 
 * minishell displays a new prompt when the signal is received.
 * 2. SIGQUIT: Ignores SIGQUIT (Ctrl + \), so that minishell doesn't quit when
 * the signal is received.
 * 3. global_signal: Is set to 0 to indicate that the heredoc process has not
 * been run (and may not even be) yet for the current command, and so
 * the execution will be run unless the heredoc process is interrupted with
 * SIGINT (Ctrl + C) when (and if) it runs.
 * 
 * The global variable g_signal is checked before the execution is launched
 * because in case it isn't 0 (but SIGUSR1), it indicates that the heredoc
 * process was interrupted with SIGINT (Ctrl + C), and so the whole command
 * should be canceled and a new prompt displayed.
*/
int	minishell_loop(t_data *data)
{
	signal(SIGINT, handle_sigint);
	g_signal = 0;
	data->prompt = set_prompt(data->env_list);
	data->input = readline((const char *)data->prompt);
	if (!check_input_null(data->input))
	{
		ft_putendl_fd("exit", STDOUT);
		exit_minishell(NULL, 0);
	}
	if (!check_quotes(data->input) || !check_enter_space(data->input))
		exit_current_prompt(data);
	data->input_split = ft_split_minishell(data->input, ' ');
	input_arr_to_lexer_list(data);
	lexer_to_simple_cmds(&data->lexer, &data->simple_cmds);
	expander(data);
	heredoc(data);
	handle_open_pipe(data);
	if (g_signal != SIGUSR1)
		exec(data, data->simple_cmds);
	exit_current_prompt(data);
	return (1);
}


int	main(int argc, char **argv, char *env[])
{
	t_data	data;

	signal(SIGQUIT, SIG_IGN);
	if (argc > 1 || argv[1])
	{
		ft_putstr_fd("Error: Minishell doesn't take any arguments.\n\n", 2);
		ft_putstr_fd("Correct usage: ./minishell\n\n", 2);
		return (0);
	}
	if (getenv("USER") == NULL)
	{
		ft_putstr_fd("Error: We truly tried but there is honestly no reliable "
			"way of constructing the minishell prompt without the USER "
			"environment variable set before minishell is run.\n", 2);
		ft_putstr_fd("Please set the variable and try again.\n\n", 2);
		return (0);
	}
	init_data(&data);
	env_init(env, &data);
	minishell_loop(&data);
	return (0);
}
