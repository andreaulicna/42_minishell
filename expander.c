/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:14:28 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/06 16:58:11 by aulicna          ###   ########.fr       */
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
	char	*tmp2;
	int	i;
	int	j;
	char	*str;
	//char	q;
	
	str = cmd[i_cmd];
//	if (str[0] != '\'' && str[0] != '"')
//		return ;
//	else
//		q = str[0];
	tmp = (char *) malloc(sizeof(ft_strlen(str) - 1));
	i = 1;
	j = 0;
	while (str[i] && str[i] != '\'' && str[i] != '"')
	{
		tmp[j] = str[i];
		//printf("%c\n", tmp[j]);
		i++;
		j++;
	}
	tmp[j] = '\0';
	tmp2 = cmd[i_cmd];
	cmd[i_cmd] = tmp;
	free(tmp2);
	ft_printf("tvl: %s\n", tmp);
	ft_printf("tvl cmd: %s\n", cmd[i_cmd]);
}

int	checker_dollar(char *str, int j)
{
	if ((str[0] == '\'' && str[ft_strlen(str) - 1] == '\''))
	{
		ft_printf("%s: NOT expand, but delete quotes if any -> \n", str);
		return 3;
	}
	else if (!str[j + 1] || str[j + 1] == ' ' || str[j + 1] == '\'' || str[j + 1] == '"')
	{
		ft_printf("%s: NOT expand, but delete quotes if any 1 ->", str);
		return 3;
	}
	else if (str[j + 1] == '?')
	{
		ft_printf("%s: expand to exit code\n", str);
		return 0;
	}
	else if (j != 0 && str[j - 1] != '\\' && str[j - 1] != '\''
		&& str[j + 1] != ' ')
	{
		ft_printf("%s: expand\n", str);
		return 0;
	}
	else if (str[j + 1] && str[j + 1] != '\'' && str[j + 1] != '"' && str[j + 1] != '\\'
		&& str[j + 1] != ' ')
	{
		ft_printf("%s: expand 1\n", str);
		return 0;
	}
	return 0;
}

void expander(t_list **simple_cmds)
{
	t_list	*current;
	t_simple_cmds	*content;
	int	i;
	int	j;

	current = *simple_cmds;
	while(current != NULL)
	{
		i = 0;
		content = (t_simple_cmds *) current->content;
		while (content->cmd[i])
		{
			j = 0;
			ft_printf("expander loop: %s\n", content->cmd[i]);
			while (content->cmd[i][j])
			{
				if (is_dollar(content->cmd[i][j]))
					if (checker_dollar(content->cmd[i], j) == 3)
					{
						delete_quotes(content->cmd, i);
						//printf("%s\n", content->cmd[i]);
					}
				j++;
			}
			i++;
		}
		ft_printf("hjlsdfh\n");
		printf("pointer: %p\n", current->next);
		t_simple_cmds *tmp_content = (t_simple_cmds *) current->next->content;
		printf("current next content: %s", tmp_content->cmd[0]);
		current = current->next;
		ft_printf("po current\n");
	}
	ft_printf("ttoto\n");
}