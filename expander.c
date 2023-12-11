/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 14:14:28 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/11 15:09:09 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"

int	is_dollar(char c)
{
	if (c == '$')
		return (1);
	return (0);
}

int	has_quotes(char *str, char *q)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\'' && str[i] != '"')
		i++;
	if (str[i])
	{
		*q = str[i];
		return (1);
	}
	else
		return (0);
}

//	Debugging printfs:
//	printf("until quotes:%s||\n", until_quotes);
//	printf("in between:%s||\n", in_between);
//	printf("rest:%s||\n", rest);

void	free_struct_str(t_str *str)
{
	if (str->part_1)
		free(str->part_1);
	if (str->part_2)
		free(str->part_2);
	if (str->part_3)
		free(str->part_3);
	if (str->tmp_join)
		free(str->tmp_join);
}

void	init_struct_str(t_str *str)
{
	str->part_1 = NULL;
	str->part_2 = NULL;
	str->part_3 = NULL;
	str->tmp_join = NULL;
	str->final = NULL;
	str->env_found = NULL;
	str->content = NULL;
}

void	delete_quotes(char **cmd, int i_cmd)
{
	char	*str;
	int		i;
	int		j;
	char	q;
	t_str	new_str;

	str = cmd[i_cmd];
	if (!has_quotes(str, &q))
		return ;
	i = 0;
	while (str[i] != q)
		i++;
	init_struct_str(&new_str);
	new_str.part_1 = ft_substr(str, 0, i);
	j = 0;
	i++;
	while (str[i + j] != q)
		j++;
	new_str.part_2 = ft_substr(str, i, j);
	new_str.part_3 = ft_substr(str, i + j + 1, ft_strlen_custom(str) - i - j);
	new_str.tmp_join = ft_strjoin(new_str.part_1, new_str.part_2);
	new_str.final = ft_strjoin(new_str.tmp_join, new_str.part_3);
	cmd[i_cmd] = new_str.final;
	free(str);
	free_struct_str(&new_str);
}

//	Debugging printfs:
//	ft_printf("%s: NOT expand, but delete quotes if any\n", str);
//	ft_printf("%s: NOT expand, but delete quotes if any 1\n", str);
//	ft_printf("%s: expand to exit code\n", str);
//	ft_printf("%s: expand\n", str);
//	ft_printf("%s: expand 1\n", str);

int	checker_dollar(char *str, int j)
{
	if ((str[0] == '\'' && str[ft_strlen(str) - 1] == '\''))
		return (4);
	else if (!str[j + 1] || str[j + 1] == ' ' || str[j + 1] == '\''
		|| str[j + 1] == '"')
		return (3);
	else if (str[j + 1] == '?')
		return (2);
	else if (j != 0 && str[j - 1] != '\\' && str[j - 1] != '\''
		&& str[j + 1] != ' ')
		return (1);
	else if (str[j + 1] && str[j + 1] != '\'' && str[j + 1] != '"'
		&& str[j + 1] != '\\' && str[j + 1] != ' ')
		return (1);
	return (0);
}

//	Debugging printfs:
//	printf("until dollar:%s||\n", until_dollar);
//	printf("key to find:%s||\n", key_to_find);
//	printf("rest:%s||\n", rest);
//
//	printf("found: %s\n", content_found->value);
//	printf("tmp1: %s\n", tmp1);
//	printf("tmp2: %s\n", tmp2);

void	expand_dollar_found(t_str *new_str)
{
	new_str->content = (t_env *) new_str->env_found->content;
	new_str->tmp_join = ft_strjoin(new_str->part_1, new_str->content->value);
	new_str->final = ft_strjoin(new_str->tmp_join, new_str->part_3);
}

void	expand_dollar(char **cmd, int i_cmd, t_list *env_list)
{
	char	*str;
	int		i;
	int		j;
	t_str	new_str;

	str = cmd[i_cmd];
	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	init_struct_str(&new_str);
	new_str.part_1 = ft_substr(str, 0, i);
	j = 0;
	while (str[i + j] && str[i + j] != ' ')
		j++;
	new_str.part_2 = ft_substr(str, i + 1, j - 1);
	new_str.part_3 = ft_substr(str, i + j, ft_strlen_custom(str) - i - j);
	new_str.env_found = env_find(env_list, new_str.part_2);
	if (new_str.env_found)
		expand_dollar_found(&new_str);
	else
		new_str.final = ft_strjoin(new_str.part_1, new_str.part_3);
	cmd[i_cmd] = new_str.final;
	free(str);
	free_struct_str(&new_str);
}

//	Debugging printfs:
//	printf("until dollar:%s||\n", until_dollar);
//	printf("rest:%s||\n", rest);
//	printf("tmp1: %s\n", tmp1);
//	printf("tmp2: %s\n", tmp2);
//
//	the exit code number needs updating with the variable from data struct

void	expand_exit_code(char **cmd, int i_cmd)
{
	char	*str;
	int		i;
	int		j;
	t_str	new_str;

	str = cmd[i_cmd];
	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	init_struct_str(&new_str);
	new_str.part_1 = ft_substr(str, 0, i);
	new_str.part_2 = ft_itoa(125);
	j = 2;
	new_str.part_3 = ft_substr(str, i + j, ft_strlen_custom(str) - i - j);
	new_str.tmp_join = ft_strjoin(new_str.part_1, new_str.part_2);
	new_str.final = ft_strjoin(new_str.tmp_join, new_str.part_3);
	cmd[i_cmd] = new_str.final;
	free(str);
	free_struct_str(&new_str);
}

int	contains_dollar(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

void	expander_loop_dollar(t_simple_cmds *content, int i, t_list *env_list)
{
	int	j;
	int	dollar_flag;

	j = 0;
	while (content->cmd[i][j])
	{
		if (is_dollar(content->cmd[i][j]))
		{
			dollar_flag = checker_dollar(content->cmd[i], j);
			delete_quotes(content->cmd, i);
			if (dollar_flag == 4)
				break ;
			else if (dollar_flag == 2)
				expand_exit_code(content->cmd, i);
			else if (dollar_flag == 1)
				expand_dollar(content->cmd, i, env_list);
		}
		j++;
	}
}

void	expander(t_list **simple_cmds, t_data *data)
{
	t_list			*current;
	t_simple_cmds	*content;
	int				i;

	current = *simple_cmds;
	while (current != NULL)
	{
		i = 0;
		content = (t_simple_cmds *) current->content;
		while (content->cmd[i])
		{
			if (contains_dollar(content->cmd[i]))
				expander_loop_dollar(content, i, data->env_list);
			else
				delete_quotes(content->cmd, i);
			i++;
		}
		current = current->next;
	}
}
