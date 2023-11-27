/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:33:13 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/27 12:59:23 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"

int main(int argc, char **argv, char *env[])
{
	(void) argc;
	(void) argv;
	char	*prompt;
	char	*input;
	int	i;

	prompt = set_prompt(env);
	while(1)
	{
		input = readline(prompt);
		if (!check_quotes(input))
		{
			printf("tu\n");
			free(input);
			continue ;
		}
		if (input == NULL)
		{
			free (prompt);
			free (input);
			printf("\nexit\n");
			exit (0);
		}
		add_history(input);
		argv = ft_split_minishell(input, ' ');
		i = 0;
		while (argv[i])
		{
			printf("%s\n", argv[i]);
			i++;
		}
		free (input);
	}
	free (prompt);
	return (0);
}