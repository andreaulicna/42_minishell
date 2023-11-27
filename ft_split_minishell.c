/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_minishell.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 19:23:37 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/27 12:36:14 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"

static int	ft_is_sep(char check, char c)
{
	if (check == c || check == '\0')
		return (1);
	else
		return (0);
}

static unsigned int	ft_count_words(char const *s, char c)
{
	unsigned int	i;
	unsigned int	num_wrds;
	unsigned int	s_quotes;
	unsigned int	d_quotes;
	unsigned int	in_wrd;

	i = 0;
	num_wrds = 0;
	s_quotes = 0;
	d_quotes = 0;
	in_wrd = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '\'')
			s_quotes++;
		else if (s[i] == '"')
			d_quotes++;
		if (!ft_is_sep(s[i], c) && in_wrd == 0)
		{
			in_wrd = 1;
			num_wrds++;
		}
		else if (ft_is_sep(s[i], c)
			&& (!(s_quotes % 2) && !(d_quotes % 2)))
			in_wrd = 0;
		i++;
	}
	printf("%d\n", num_wrds);
	return (num_wrds);
}

static void	ft_fill_arr(char *sub_arr, char const *s, char c)
{
	unsigned int	i;

	i = 0;
	while (ft_is_sep(s[i], c) == 0)
	{
		sub_arr[i] = s[i];
		i++;
	}
	sub_arr[i] = '\0';
}

static void	ft_alloc_fill_arr(char **arr, char const *s, char c)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	wrds;

	i = 0;
	wrds = 0;
	while (s[i] != '\0')
	{
		if (ft_is_sep(s[i], c) == 1)
		{
			i++;
		}
		else
		{
			j = 0;
			while (ft_is_sep(s[i + j], c) == 0)
			{
				j++;
			}
			arr[wrds] = (char *) malloc(sizeof(char) * (j + 1));
			ft_fill_arr(arr[wrds], &s[i], c);
			i += j;
			wrds++;
		}
	}
}

char	**ft_split_minishell(char const *s, char c)
{
	unsigned int	num_wrds;
	char			**arr;

	if (!s)
		return (NULL);
	num_wrds = ft_count_words(s, c);
	arr = (char **) malloc(sizeof(char *) * (num_wrds + 1));
	if (!arr)
		return (NULL);
	ft_alloc_fill_arr(arr, s, c);
	arr[num_wrds] = 0;
	return (arr);
}
