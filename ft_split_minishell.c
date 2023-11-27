/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_minishell.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 19:23:37 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/27 20:03:56 by aulicna          ###   ########.fr       */
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

void	count_qoutes(char c, unsigned int *s_quotes, unsigned int *d_quotes)
{
	if (c == '\'' && !(*d_quotes % 2))
		*s_quotes += 1;
	else if (c == '"' && !(*s_quotes % 2))
		*d_quotes += 1;
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
	while (s[i])
	{
		count_qoutes(s[i], &s_quotes, &d_quotes);
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

static char	*ft_fill_arr(char const *s, unsigned int start,
	unsigned int end)
{
	unsigned int	i;
	char			*sub_arr;

	sub_arr = (char *) malloc(sizeof(char) * (end - start + 1));
	i = 0;
	while (start < end && s[i])
	{
		sub_arr[i] = s[start];
		i++;
		start++;
	}
	sub_arr[i] = '\0';
	return (sub_arr);
}

static void	ft_alloc_fill_arr(char **arr, char const *s, char c)
{
	int				i;
	unsigned int	wrds;
	unsigned int	s_quotes;
	unsigned int	d_quotes;
	int				in_wrd;

	i = 0;
	wrds = 0;
	s_quotes = 0;
	d_quotes = 0;
	in_wrd = -1;
	while (i <= (int)ft_strlen(s))
	{
		count_qoutes(s[i], &s_quotes, &d_quotes);
		if (!ft_is_sep(s[i], c) && in_wrd == -1)
			in_wrd = i;
		else if ((ft_is_sep(s[i], c) || i == (int)ft_strlen(s)) && in_wrd >= 0
			&& !(s_quotes % 2) && !(d_quotes % 2))
		{
				arr[wrds++] = ft_fill_arr(s, in_wrd, i);
				in_wrd = -1;
		}
		i++;
	}
	arr[wrds] = '\0';
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
	return (arr);
}
