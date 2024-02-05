/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:33:13 by aulicna           #+#    #+#             */
/*   Updated: 2024/02/05 23:19:00 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"

int	g_signal = 0;

/**
 * @brief	Runs the minishell.
 * 
 * Signals management:
 * SIGINT signal is handled via the primary SIGINT handler which displays a new
 * prompt and sets g_signal to SIGINT so that this value cam inform the exit
 * status used in the expander.
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
	if (g_signal == 0)
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
