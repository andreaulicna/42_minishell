/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 14:33:13 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/06 16:56:15 by aulicna          ###   ########.fr       */
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

void	free_lexer(t_list **lexer)
{
	t_list	*tmp;
	t_lexer	*content;

	if (!*lexer)
		return ;
	while (*lexer)
	{
		tmp = (*lexer)->next;
		content = (t_lexer *) (*lexer)->content;
		if (content->word)
			free(content->word);
		free(content);
		free(*lexer);
		*lexer = tmp;
	}
	lexer = NULL;
}


void	free_simple_cmds(t_list **simple_cmds)
{
	t_simple_cmds *content;
	t_list	*free_redirects;
	t_list	*tmp;

	if (!simple_cmds)
		return ;
	while (*simple_cmds)
	{
		tmp = (*simple_cmds)->next;
		content = (t_simple_cmds *) (*simple_cmds)->content;
		free_redirects = content->redirects;
		free_lexer(&free_redirects);
		free_arr(content->cmd);
		free(content);
		free(*simple_cmds);
		*simple_cmds = tmp;
	}
	simple_cmds = NULL;
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
		printf("pointer: %p\n", current->next);
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
			printf("Redirect word: %s\n", content_redirects->word + 1);
			current_redirect = current_redirect->next;
		}
		current = current->next;
		order++;
	}
}

int main(int argc, char **argv, char *env[])
{
	(void) argc;
	(void) argv;
	char	**input_split;
	char	*prompt;
	char	*input;
	//int	i;
	unsigned int	null_input;
	t_list	*lexer;
	t_list	*simple_cmds;

	prompt = set_prompt(env);
	null_input = 0;
	while(1)
	{
		input = readline((const char *)prompt);
		if (!check_input_null(input))
		{
			null_input = 1;
			free(input);
			break ;
		}
		if (!check_quotes(input))
		{
			free(input);
			continue ;
		}
		/* Split S*/
		input_split = ft_split_minishell(input, ' ');
	//	i = 0;
	//	printf("Minishell split output:\n");
	//	while (input_split[i])
	//	{
	//		printf("%s\n", input_split[i]);
	//		i++;
	//	}
	//	printf("----------------------\n");
		/* Split E */
		/* Lexer - Link list S */
	//	printf("Minishell lexer output:\n");
		lexer = input_arr_to_lexer_list(input_split);
		free_arr(input_split);
		print_lexer(&lexer);
	//	printf("----------------------\n");
		/* Lexer - Link list E */
		/* Parser - Link list S */
	//	printf("Minishell parser output:\n");
		simple_cmds = lexer_to_simple_cmds(&lexer);
	//	printf("Rest of lexer: \n");
	//	print_lexer(&lexer);
	//	printf("----------------------\n");
	//	printf("SIMPLE CMDS - before expander\n");
	//	print_simple_cmds(&simple_cmds);
	//	printf("SIMPLE CMDS - after expander\n");
		print_simple_cmds(&simple_cmds);
		printf("----------------------\n");
		expander(&simple_cmds);
		ft_printf("tu\n");
		//print_simple_cmds(&simple_cmds);
		//printf("----------------------\n");
		/* Parser - Link list E */
	//	free(input);
	//	free_lexer(&lexer);
	//	free_simple_cmds(&simple_cmds);
	}
	//free(prompt);
	if (null_input == 1)
	{
		printf("\nexit\n");
		exit(0);
	}
	return (0);
}