/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbartos <vbartos@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:26:36 by vbartos           #+#    #+#             */
/*   Updated: 2023/12/04 13:36:34 by vbartos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void deleteList(t_list **head_ref)
{
   t_list *current = *head_ref;
   t_list *next;

   while (current != NULL) 
   {
		next = current->next;
		free(current->content);
		free(current);
		current = next;
   }
   *head_ref = NULL;
}

int main(int argc, char **argv, char **envp)
{
	t_data	data;
	// char	*prompt;
	// char	*cmd;
	// size_t	n;

	// prompt = PROMPT;
	// cmd = NULL;
	// n = 0;
	// while (1)
	// {
	// 	ft_pwd();
	// 	printf("%s", prompt);
	// 	getline(&cmd, &n, stdin);
	// 	ft_cd(cmd);
	// }
	// free(cmd);

	(void) argc;
	(void) argv;
	// (void) envp;

	char *export_cmd[] = {"export", NULL};
	char *export_cmd2[] = {"export", "AAAA", "hehe=lol", "zzabijak=hehe", NULL};
	// char *unset_cmd[] = {"unset", NULL};
	char *unset_cmd2[] = {"unset", "COLORTRM", "DISPLAY", NULL};

	env_init(envp, &data);
	// char *cd_cmd[] = {"cd", NULL};
	// char *cd_cmd2[] = {"cd", "../..", "haha" , NULL};
	// ft_env(&data);
	printf("\n\n");
	// ft_cd(cd_cmd2, &data);
	printf("\n\n");
	// ft_pwd();
	// ft_cd(cd_cmd, &data);
	// ft_pwd();
	printf("\n\n");
	// ft_env(&data);
	ft_export(export_cmd, &data);
	// ft_unset(unset_cmd, &data);
	ft_unset(unset_cmd2, &data);
	ft_printf("\n\n\n");
	ft_export(export_cmd2, &data);
	// ft_export(export_cmd, &data);
	// while (current != NULL)
	// {
	// 	ft_printf("%s\n", current->content);
	// 	current = current->next;
	// }
	// ft_printf("\n\n\n");
	// current = data.env_list;
	// env_remove(&current, "lol");
	// while (current != NULL)
	// {
	// 	ft_printf("%s\n", current->content);
	// 	current = current->next;
	// }

	// char *echo_cmd[] = {"echo", "ahojda", "kamarade", NULL};
	// char *echo_cmd2[] = {"echo", "-f", "ahojda", "kamarade", NULL};
	// ft_echo(echo_cmd2);
	// ft_printf("\n\n\n");
	// ft_pwd();
	// ft_printf("\n\n\n");
	// ft_env(envp);
	// ft_printf("\n\n\n");

	// char *cd_cmd[] = {"cd", "/home/hackerman/", NULL};
	// char *cd_cmd2[] = {"cd", "/home/hackerman/minishell/", NULL};
	// char *cd_cmd3[] = {"cd", "..", NULL};
	// ft_pwd();
	// ft_cd(cd_cmd);
	// ft_cd(cd_cmd2);
	// ft_cd(cd_cmd3);
	// ft_env(envp);
	deleteList(&data.env_list);
	return (0);
}