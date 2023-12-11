/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:33:13 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/11 21:09:37 by aulicna          ###   ########.fr       */
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
	else
		add_history(input);
	return (1);
}

void	print_lexer(t_list **lexer)
{
	t_lexer	*content;
	t_list *current;
	
	current = *lexer;
	while (current != NULL)
	{
		content = (t_lexer *) (current->content);
		if (content->token)
			ft_printf("token: %d\n", content->token);
		else
			ft_printf("word: %s\n", content->word);
		current = current->next;
	}
}

void	print_simple_cmds(t_list **simple_cmds)
{
	t_list	*current;
	t_simple_cmds *content_simple_cmds;
	t_lexer *content_redirects;
	t_list	*current_redirect;
	int	i;
	int	order;

	current = *simple_cmds;
	order = 0;
	while (current)
	{
		printf("%s %d\n", "SIMPLE CMDS", order);
		content_simple_cmds = (t_simple_cmds *) current->content;
		current_redirect = content_simple_cmds->redirects;
		printf("\n***Cmds***\n");
		i = 0;
		while (content_simple_cmds->cmd[i])
		{
			printf("%s\n", content_simple_cmds->cmd[i]);
			i++;
		}
		printf("\n***Redirects***\n");
		while (current_redirect)
		{
			content_redirects = (t_lexer *) current_redirect->content;
			printf("Redirect token: %d\n", content_redirects->token);
			printf("Redirect word: %s\n", content_redirects->word);
			current_redirect = current_redirect->next;
		}
		current = current->next;
		order++;
	}
}

void	init_data(t_data *data)
{
	data->env_list = NULL;
	data->lexer = NULL;
	data->simple_cmds = NULL;
	data->prompt = NULL;
	data->input = NULL;
	data->input_split = NULL;
}

int main(int argc, char **argv, char *env[])
{
	t_data	data;
	
	(void) argc;
	(void) argv;
	int	i;
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
		data.prompt = set_prompt(env);
		data.input = readline((const char *)data.prompt);
		if (!check_input_null(data.input))
		{
			null_input = 1;
			//free(data.input);
			free_data(&data);
			break ;
		}
		if (!check_quotes(data.input))
		{
			//free(data.input);
			free_data(&data);
			continue ;
		}
		/* Split S*/
		data.input_split = ft_split_minishell(data.input, ' ');
		i = 0;
		printf("Minishell split output:\n");
		while (data.input_split[i])
		{
			printf("%s\n", data.input_split[i]);
			i++;
		}
		printf("----------------------\n");
		/* Split E */
		/* Lexer - Link list S */
		printf("Minishell lexer output:\n");
		data.lexer = input_arr_to_lexer_list(data.input_split);
		print_lexer(&data.lexer);
		printf("----------------------\n");
		/* Lexer - Link list E */
		/* Parser - Link list S */
		printf("Minishell parser output:\n");
		data.simple_cmds = lexer_to_simple_cmds(&data.lexer);
		printf("Rest of lexer: \n");
		print_lexer(&data.lexer);
		printf("----------------------\n");
		printf("SIMPLE CMDS - before expander\n");
		print_simple_cmds(&data.simple_cmds);
		printf("----------------------\n");
		printf("SIMPLE CMDS - after expander\n");
		expander(&data);
		print_simple_cmds(&data.simple_cmds);
		printf("----------------------\n");
		/* Parser - Link list E */
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