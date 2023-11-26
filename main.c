/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:33:13 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/26 19:29:57 by aulicna          ###   ########.fr       */
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

	while(1)
	{
		prompt = set_prompt(env);
		input = readline(prompt);
		add_history(input);
		argv = ft_split_minishell(input, ' ');
		i = 0;
		while (argv[i])
		{
			printf("%s\n", argv[i]);
			i++;
		}
		if (input == NULL)
		{
			free (prompt);
			free (input);
			printf("\nexit\n");
			exit (0);
		}
		free (prompt);
	}
	free (input);
	return (0);
}