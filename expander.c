/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:14:28 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/06 14:33:43 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"

int	is_dollar(char c)
{
	if (c == '$')
		return (1);
	return (0);
}

void	delete_quotes(char **cmd, int i_cmd)
{
	char	*tmp;
	int	i;
	int	j;
	char	*str = cmd[i_cmd];

	tmp = (char *) malloc(sizeof(ft_strlen(str) - 1));
	i = 1;
	j = 0;
	while (str[i] != '\'' && str[i] != '"' && str[i])
	{
		tmp[j] = str[i];
		i++;
		j++;
	}
	tmp[j] = '\0';
	free(cmd[i_cmd]);
	cmd[i_cmd] = tmp;
}

int	checker_dollar(char *str, int j)
{
	if ((str[0] == '\'' && str[ft_strlen(str) - 1] == '\''))
	{
		printf("%s: NOT expand, but delete quotes if any -> ", str);
		return 3;
	}
	else if (!str[j + 1] || str[j + 1] == ' ' || str[j + 1] == '\'' || str[j + 1] == '"')
	{
		printf("%s: NOT expand, but delete quotes if any ->", str);
		return 3;
	}
	else if (str[j + 1] == '?')
	{
		printf("%s: expand to exit code\n", str);
		return 0;
	}
	else if (str[j + 1] && str[j + 1] != '\'' && str[j + 1] != '"' && str[j + 1] != '\\'
		&& str[j + 1] != ' ')
	{
		printf("%s: expand 1\n", str);
		return 0;
	}
	else if (j != 0 && str[j - 1] != '\\' && str[j - 1] != '\''
		&& str[j + 1] != ' ')
	{
		printf("%s: expand\n", str);
		return 0;
	}
	//printf("%s: NOT expand\n", str);
	return 0;
}

void expander(t_list **simple_cmds)
{
	t_list	*current;
	t_simple_cmds	*content;
	int	i;
	int	j;

	current = *simple_cmds;
	while(current)
	{
		i = 0;
		content = (t_simple_cmds *) current->content;
		while (content->cmd[i])
		{
			j = 0;
			while (content->cmd[i][j])
			{
				if (is_dollar(content->cmd[i][j]))
					if (checker_dollar(content->cmd[i], j) == 3)
					{
						delete_quotes(content->cmd, i);
						printf("%s\n", content->cmd[i]);
					}
				j++;
			}
			i++;
		}
		current = current->next;
	}
}