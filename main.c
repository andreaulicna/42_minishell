/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:33:13 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/27 19:17:42 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	check_input_null(char *input)
{
	if (input == NULL)
		return (0);
	else if (input[0])
		add_history(input);
	return (1);
}

int main(int argc, char **argv, char *env[])
{
	(void) argc;
	(void) argv;
	char	**arr;
	char	*prompt;
	char	*input;
	int	i;
	unsigned int	null_input;

	prompt = set_prompt(env);
	null_input = 0;
	while(1)
	{
		input = readline((const char *)prompt);
		if (!check_input_null(input))
		{
			null_input = 1;
			break ;
		}
		if (!check_quotes(input))
		{
			free(input);
			continue ;
		}
		arr = ft_split_minishell(input, ' ');
		i = 0;
		while (arr[i])
		{
			printf("%s\n", arr[i]);
			i++;
		}
		free_arr(arr);
	}
	free (prompt);
	free (input);
	if (null_input == 1)
	{
		printf("\nexit\n");
		exit(0);
	}
	return (0);
}