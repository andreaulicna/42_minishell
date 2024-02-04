/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 10:28:47 by aulicna           #+#    #+#             */
/*   Updated: 2024/02/04 18:08:40 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @file	This file includes functions that were used for debugging. Their
 * are not included in the compilation as they aren't used during normal
 * execution of the minishell.
*/

void	print_input_split(char **input_split)
{
	int	i;

	i = 0;
	while (input_split[i])
	{
		printf("%s\n", input_split[i]);
		i++;
	}
}

void	print_lexer(t_list **lexer)
{
	t_lexer	*content;
	t_list	*current;

	current = *lexer;
	while (current != NULL)
	{
		content = (t_lexer *)(current->content);
		if (content->token)
			ft_printf("token: %d\n", content->token);
		else
			ft_printf("word: %s\n", content->word);
		current = current->next;
	}
}

static void	print_redirects(t_list *current_redirect)
{
	t_lexer			*content_redirects;

	printf("\n***Redirects***\n");
	while (current_redirect)
	{
		content_redirects = (t_lexer *) current_redirect->content;
		printf("Redirect token: %d\n", content_redirects->token);
		printf("Redirect word: %s\n", content_redirects->word);
		current_redirect = current_redirect->next;
	}
}

void	print_simple_cmds(t_list **simple_cmds)
{
	t_list			*current;
	t_simple_cmds	*content_simple_cmd;
	int				i;
	int				order;

	current = *simple_cmds;
	order = 0;
	while (current)
	{
		printf("\n%s %d\n\n%s\n", "SIMPLE CMDS", order, "***Cmds***");
		content_simple_cmd = (t_simple_cmds *) current->content;
		i = 0;
		while (content_simple_cmd->cmd[i])
		{
			printf("%s\n", content_simple_cmd->cmd[i]);
			i++;
		}
		print_redirects(content_simple_cmd->redirects);
		printf("\n***Hd_file***\n");
		if (content_simple_cmd->hd_file)
			printf("%s\n", content_simple_cmd->hd_file);
		printf("\n");
		current = current->next;
		order++;
	}
}

//int	minishell_loop(t_data *data)
//{
//	signal(SIGINT, handle_sigint);
//	signal(SIGQUIT, SIG_IGN);
//	g_signal = 0;
//	data->prompt = set_prompt(data->env_list);
//	data->input = readline((const char *)data->prompt);
//	if (!check_input_null(data->input))
//	{
//		ft_putendl_fd("exit", STDOUT);
//		exit_minishell(NULL, 50);
//	}
//	if (!check_quotes(data->input) || !check_enter_space(data->input))
//		exit_current_prompt(data);
//	/* Split S*/
//	data->input_split = ft_split_minishell(data->input, ' ');
//	printf("Minishell split output:\n");
//	print_input_split(data->input_split);
//	printf("----------------------\n");
//	/* Split E */
//	/* Lexer - Link list S */
//	printf("Minishell lexer output:\n");
//	input_arr_to_lexer_list(data);
//	print_lexer(&data->lexer);
//	printf("----------------------\n");
//	/* Lexer - Link list E */
//	/* Parser - Link list S */
//	printf("Minishell parser output:\n");
//	printf("----------------------\n");
//	lexer_to_simple_cmds(&data->lexer, &data->simple_cmds);
//	printf("Rest of lexer: \n");
//	print_lexer(&data->lexer);
//	/* Parser - Link list E */
//	printf("----------------------\n");
//	printf("SIMPLE CMDS - before expander\n");
//	print_simple_cmds(&data->simple_cmds);
//	printf("----------------------\n");
//	expander(data);
//	printf("SIMPLE CMDS - after expander\n");
//	print_simple_cmds(&data->simple_cmds);
//	printf("----------------------\n");
//	printf("Heredoc output:\n");
//	heredoc(data);
//	printf("----------------------\n");
//	printf("SIMPLE CMDS - after heredoc\n");
//	handle_open_pipe(data);
//	if (g_signal == 0)
//		exec(data, data->simple_cmds);
//	exit_current_prompt(data);
//	return (1); //should never reach this
//}
