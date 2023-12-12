/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_minishell.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 19:23:37 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/12 11:36:29 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/**
 * @brief	Checks whether a character is a separator or a null terminator.
 * 
 * @param	check	character to check
 * @param	c		separator character
 * @return 	int		1 if the character is a separator or '\0', otherwise 0
 */

static int	ft_is_sep(char check, char c)
{
	if (check == c || check == '\0')
		return (1);
	else
		return (0);
}

/**
 * @brief	Counts the number of words (tokens) in the input string,
 * taking into account single and double quotes as potential delimiters.
 * 
 * It uses a variable in_wrd to signify when it is parsing a potential word.
 * Once the function locates a separator it checks whether the quotes are paired
 * and if so, it resets the in_wrd variable to start the processing next word.
 * 
 * @param	s				input string
 * @param	c				delimiter character
 * @return	unsigned int	the number of words in the input string
 */

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
		else if (ft_is_sep(s[i], c) && quotes_pair(s_quotes, d_quotes))
			in_wrd = 0;
		i++;
	}
	return (num_wrds);
}

/**
 * @brief	Fills an array with characters from a substring of a string.
 * 
 * @param	s		input string
 * @param	start	starting index of the substring
 * @param	end		ending index of the substring
 * @return	char*	dynamically allocated string containing the substring
 */

static char	*ft_fill_arr(char const *s, unsigned int start,
	unsigned int end)
{
	unsigned int	i;
	char			*sub_arr;

	sub_arr = (char *) malloc(sizeof(char) * (end - start + 1));
	if (!sub_arr)
		return (NULL);
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

/**
 * @brief	Allocates and fills an array with substrings separated by 
 * a delimiter character.
 * 
 * It uses a variable in_wrd to signify when it is parsing a potential word. The
 * beginning of this word is saved in in_wrd.
 * 
 * Once the function locates a separator it checks whether the quotes are paired
 * and if so, it fills in the word and resets the in_wrd variable to start
 * processing the next word.
 * 
 * @param	arr	array to store the substrings
 * @param 	s 	input string
 * @param	c	delimiter character
 */

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
			&& quotes_pair(s_quotes, d_quotes))
		{
				arr[wrds++] = ft_fill_arr(s, in_wrd, i);
				in_wrd = -1;
		}
		i++;
	}
	arr[wrds] = '\0';
}

/**
 * @brief	Splits a string into an array of substrings based on a delimiter
 * character, considering single and double quotes in the input string.
 * 
 * @param	s		input string
 * @param	c		delimiter character
 * @return	char**	dynamically allocated array containing the substrings
 */

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
