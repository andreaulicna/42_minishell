/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:33:13 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/28 15:55:50 by aulicna          ###   ########.fr       */
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
	char	**input_split;
	char	*prompt;
	char	*input;
	int	i;
	unsigned int	null_input;
	t_lexer	*lexer;
	//t_lexer *current;

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
		/* Split S*/
		input_split = ft_split_minishell(input, ' ');
		i = 0;
		printf("Minishell split output:\n");
		while (input_split[i])
		{
			printf("%s\n", input_split[i]);
			i++;
		}
		printf("----------------------\n");
		/* Split E */
		/* Lexer - Expand S */
		printf("Minishell lexer output:\n");
		lexer = input_arr_to_lexer_list(input_split);
		//free_arr(input_split);
	//	current = lexer->next;
	//	while(current->next)
	//	{
	//		if (current->word)
	//			printf("word: %s\n", current->word);
	//		else if (current->token)
	//			printf("token: %d\n", current->token);
	//		current = current->next;
	//	}
	//	if (current->word)
	//		printf("word: %s\n", current->word);
	//	else if (current->token)
	//		printf("token: %d\n", current->token);
//		current = lexer;
//		while (current != NULL)
//		{
//			ft_printf("wordv: %s\n", current->word);
//			current = current->next;
//		}
//		printf("id 0: %d\n", lexer->id);
//		printf("word 0: %s\n", lexer->word);
		//printf("id 1: %d\n", lexer->next->id);
		//printf("word 0: %s\n", lexer->next->word);
		while (lexer)
		{
			if (lexer->word)
				printf("word: %s\n", lexer->word);
			else if (lexer->token)
				printf("token: %d\n", lexer->token);
			lexer = lexer->next;
		}
		printf("----------------------\n");
		/* Lexer - Expand E */
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