/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_a.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:33:13 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/16 10:30:06 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

int	check_input_null(char *input)
{
	if (input == NULL)
		return (0);
	else
		add_history(input);
	return (1);
}

int main(int argc, char **argv, char *env[])
{
	t_data	data;
	
	(void) argc;
	(void) argv;
	unsigned int	null_input;

	null_input = 0;
	init_data(&data);
	if (argc > 1)
	{
		ft_putstr_fd("Error: Minishell doesn't take any arguments.\n\n", 2);
		ft_putstr_fd("Correct usage: ./minishell\n\n", 2);
		return (0);
	}
	while(1)
	{
		env_init(env, &data); //either init in every run or change the free function
		data.prompt = set_prompt(data.env_list);
		data.input = readline((const char *)data.prompt);
		if (!check_input_null(data.input))
		{
			null_input = 1;
			free_data(&data);
			break ;
		}
		if (!check_quotes(data.input))
		{
			free_data(&data);
			continue ;
		}
		data.input_split = ft_split_minishell(data.input, ' ');
		data.lexer = input_arr_to_lexer_list(data.input_split);
		data.simple_cmds = lexer_to_simple_cmds(&data.lexer);
		expander(&data);
		// execute cmds
		free_data(&data);
	}
	//free(data.prompt); // needs to be here, bcs check_input skips the free_data function and the one thing that stays malloced is the prompt
	if (null_input == 1)
	{
		printf("\nexit\n");
		exit(0);
	}
	return (0);
}