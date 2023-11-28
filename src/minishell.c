/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:26:36 by vbartos           #+#    #+#             */
/*   Updated: 2023/11/28 06:30:52 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int main(int argc, char **argv, char **envp)
{
	char	*prompt;
	char	*cmd;
	size_t	n;

	prompt = PROMPT;
	cmd = NULL;
	n = 0;
	while (1)
	{
		printf("%s", prompt);
		getline(&cmd, &n, stdin);
		ft_cd(cmd);
	}
	free(cmd);

	(void) argc;
	(void) argv;

	// char *echo_cmd[] = {"echo", "ahojda", "kamarade", NULL};
	// char *echo_cmd2[] = {"echo", "-n", "ahojda", "kamarade", NULL};
	// ft_echo(echo_cmd2);
	// ft_printf("\n\n\n");
	// ft_pwd();
	// ft_printf("\n\n\n");
	// ft_env(envp);
	// ft_printf("\n\n\n");
	return (0);
}