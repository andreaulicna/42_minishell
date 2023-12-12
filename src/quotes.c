/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 12:35:35 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/12 11:41:31 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/**
 * @brief	Checks if a string contains quotes and determines the quote type. 
 * If quotes are found, it updates the 'q' parameter with the type encountered.
 * 
 * @param	str	string to be checked for quotes
 * @param	q	pointer to a character variable that stores the quote type found
 * @return	int	returns 1 if quotes are found; 0 otherwise
 */

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

/**
 * @brief	Deletes the outermost quotes from a string.
 * 
 * The functions first checks whether there are some quotes to remove using 
 * the has_quotes function. 
 * 
 * Then it breaks down the input string into 3 parts:
 * part_1: up until, but not including, the first occurrence of the outermost
 * pair of quotes
 * part_2: up until, but not including, the second occurrence of the outermost
 * part of quotes, i.e. in between the outermost pair of quotes
 * part_3: after the second occurrence of the outermost pair of quotes
 * 
 * And then it constructs the final string in 2 steps, first joining part_1 and
 * part_2 into tmp_join and then joining tmp_join and part_3 into final.
 * 
 * Finally, the appropriate string in the 2D input array is pointed to the final
 * string. All of the dynamically allocated memory is then freed with the
 * exception of the final string that is not freed until after the whole command
 * is processed and the program reaches the free_simple_cmds function.
 * 
 * @param 	cmd		array of strings containing the input commands
 * @param	i_cmd	index of the command string in the array to be modified
 */

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

/**
 * @brief	Counts single and double quotes in a string.
 * 
 * This function keeps track of the number of single and double quotes
 * encountered in the input string. It's used in the word-counting process
 * to correctly handle quoted substrings as part of a word.
 * 
 * @param	c			character to check
 * @param	s_quotes	pointer to the count of single quotes
 * @param	d_quotes	pointer to the count of double quotes
 */

void	count_qoutes(char c, unsigned int *s_quotes, unsigned int *d_quotes)
{
	if (c == '\'' && !(*d_quotes % 2))
		*s_quotes += 1;
	else if (c == '"' && !(*s_quotes % 2))
		*d_quotes += 1;
}

/**
 * @brief	Checks if both single and double quotes are paired.
 * 
 * This function determines whether both single and double quotes are paired in
 * a balanced manner. It's crucial for identifying if a word is enclosed in
 * a pair of matched quotes or not.
 * 
 * @param	s_quotes	count of single quotes
 * @param	d_quotes	count of double quotes
 * @return	int			1 if single and double quotes are paired, otherwise 0
 */

int	quotes_pair(unsigned int s_quotes, unsigned int d_quotes)
{
	if (!(s_quotes % 2) && !(d_quotes % 2))
		return (1);
	return (0);
}

/**
 * @brief	Checks for the presence and closure of quotes in the input string.
 * 
 * It prints an error message to STDERR if unclosed quotes are found.
 * 
 * @param	input	input string to be checked for quotes
 * @return	int		returns 1 if all quotes are closed and balanced; 0 otherwise
 */

int	check_quotes(char *input)
{
	int				i;
	unsigned int	quotes;

	quotes = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' && quotes == 0)
			quotes = 1;
		else if (input[i] == '\'' && quotes == 1)
			quotes = 0;
		else if (input[i] == '"' && quotes == 0)
			quotes = 2;
		else if (input[i] == '"' && quotes == 2)
			quotes = 0;
		i++;
	}
	if (quotes != 0)
	{
		ft_putstr_fd("Minishell cannot interpret unclosed quotes, ", 2);
		ft_putstr_fd("please close them.\n", 2);
		return (0);
	}
	return (1);
}
