/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:14:28 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/08 15:26:01 by aulicna          ###   ########.fr       */
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
	char	*str;
	int	i;
	int	j;
	char q;
	char	*tmp;

	str = cmd[i_cmd];
	i = 0;
	while (str[i] && str[i] != '\'' && str[i] != '"')
		i++;
	if (str[i])
		q = str[i];
	else
		return ;
	tmp = (char *) malloc(sizeof(char) * (ft_strlen(str) - 1));
	i = 0;
	j = 0;
	while (str[i] != q)
	{
		tmp[j] = str[i];
		i++;
		j++;
	}
	i++;
	while (str[i] != q)
	{
		tmp[j] = str[i];
		i++;
		j++;
	}
	i++;
	while (str[i])
	{
		tmp[j] = str[i];
		i++;
		j++;
	}
	tmp[j] = '\0';
	cmd[i_cmd] = tmp;
	free(str);
}

int	checker_dollar(char *str, int j)
{
	if ((str[0] == '\'' && str[ft_strlen(str) - 1] == '\''))
	{
		ft_printf("%s: NOT expand, but delete quotes if any\n", str);
		return 4;
	}
	else if (!str[j + 1] || str[j + 1] == ' ' || str[j + 1] == '\'' || str[j + 1] == '"')
	{
		ft_printf("%s: NOT expand, but delete quotes if any 1\n", str);
		return 3;
	}
	else if (str[j + 1] == '?')
	{
		ft_printf("%s: expand to exit code\n", str);
		return 2;
	}
	else if (j != 0 && str[j - 1] != '\\' && str[j - 1] != '\''
		&& str[j + 1] != ' ')
	{
		ft_printf("%s: expand\n", str);
		return 1;
	}
	else if (str[j + 1] && str[j + 1] != '\'' && str[j + 1] != '"' && str[j + 1] != '\\'
		&& str[j + 1] != ' ')
	{
		ft_printf("%s: expand 1\n", str);
		return 1;
	}
	return 0;
}

void	expand_dollar(char **cmd, int i_cmd, t_list *env_list)
{
	//char	*key = "USER";
	//char	*value = "aulicna";
	char	*str;
	int	i;
	int	j;
	char	*until_dollar;
	char	*key_to_find;
	char	*rest;
	char	*tmp1;
	char	*tmp2;
	t_list	*found;
	t_env	*content_found;

	str = cmd[i_cmd];
	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	until_dollar = ft_substr(str, 0, i);
	printf("until dollar:%s||\n", until_dollar);
	j = 0;
	while (str[i + j] && str[i + j] != ' ')
		j++;
	key_to_find = ft_substr(str, i + 1, j - 1);
	printf("key to find:%s||\n", key_to_find);
	rest = ft_substr(str, i + j, ft_strlen_custom(str) - i - j);
	printf("rest:%s||\n", rest);
	//printf("key:%s||\n", key);
	found = env_find(env_list, key_to_find);
	if (!found)
	{
		printf("JOU\n");
	}
	content_found = (t_env *) found->content;
	printf("found: %s\n", content_found->value);
	if (found)
	{
		tmp1 = ft_strjoin(until_dollar, content_found->value);
		printf("tmp1: %s\n", tmp1);
		tmp2 = ft_strjoin(tmp1, rest);
		printf("tmp2: %s\n", tmp2);
		free(tmp1);
	}
//	if (!ft_strncmp(key, key_to_find, 4))
//	{
//		tmp1 = ft_strjoin(until_dollar, value);
//		printf("tmp1: %s\n", tmp1);
//		tmp2 = ft_strjoin(tmp1, rest);
//		printf("tmp2: %s\n", tmp2);
//		free(tmp1);
//	}
	else
	{
		tmp2 = ft_strjoin(until_dollar, rest);
		printf("tmp2: %s\n", tmp2);
	}
	cmd[i_cmd] = tmp2;
	free(str);
	free(until_dollar);
	free(key_to_find);
	free(rest);
}

void	expand_exit_code(char **cmd, int i_cmd)
{
	char *tmp;
	char *str;

	tmp = ft_strdup("**exit code**");
	str = cmd[i_cmd];
	cmd[i_cmd] = tmp;
	free(str);
}

void expander(t_list **simple_cmds, t_data *data)
{
	t_list	*current;
	t_simple_cmds	*content;
	int	i;
	int	j;
	int	check_dollar;

	current = *simple_cmds;
	while(current != NULL)
	{
		i = 0;
		content = (t_simple_cmds *) current->content;
		while (content->cmd[i])
		{
			j = 0;
			while (content->cmd[i][j])
			{
				if (is_dollar(content->cmd[i][j]))
				{
					check_dollar = checker_dollar(content->cmd[i], j);
					if (check_dollar == 4)
					{
						delete_quotes(content->cmd, i);
						break ;
					}
					else if (check_dollar == 3)
						delete_quotes(content->cmd, i);
					else if (check_dollar == 2)
					{
						delete_quotes(content->cmd, i);
						expand_exit_code(content->cmd, i);
					}
					else if (check_dollar == 1)
					{
						delete_quotes(content->cmd, i);
						expand_dollar(content->cmd, i, data->env_list);
					}
				}
				j++;
			}
			delete_quotes(content->cmd, i);
			i++;
		}
		current = current->next;
	}
}