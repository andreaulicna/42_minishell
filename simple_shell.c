/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_shell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:55:35 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/22 12:58:40 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"
#include "libftprintf/ft_printf.h"

void execmd(char **arr)
{
	char	*command;

	if (arr != NULL)
	{
		command = arr[0];
		if (execve(command, arr, NULL) == -1)
			perror("Error:");
	}
}


int	main(int argc, char **argv)
{

	char	*prompt;
	char	*lineptr;
	size_t	n;
	__ssize_t	nchars_read;
	char	*lineptr_copy;
	const char	*delim = " \n";
	char	*token;
	int	num_tokens;
	char	**arr;
	int	i;

	(void) argc;
	(void) argv;
	while (1)
	{
		prompt = "simple-shell\% ";
		printf("%s", prompt);
		n = 0;
		lineptr = NULL;
		nchars_read = getline(&lineptr, &n, stdin);
		/* Dynamically allocate space to store a copy of the string read */
		lineptr_copy = (char *) malloc(sizeof(char) * nchars_read);
		if (lineptr_copy == NULL)
		{
			perror("tsh: memory allocation error");
			return (-1);
		}
		/* Make a copy of the lineptr */
		strcpy(lineptr_copy, lineptr);
		if (nchars_read == -1)
		{
			printf("exit\n");
			return (-1);
		}
		else
		{
			/* Calculate the total number of tokens expected from the string */
			/* split the string (lineptr) into an array of words */
			token = strtok(lineptr, delim);

			/* determine how many tokens are there*/
			num_tokens = 0;
			while (token != NULL)
			{
				token = strtok(NULL, delim);
				num_tokens++;
			}
			num_tokens++;
	
			/* Allocate space to hold the array of strings (tokens) */
			arr = (char **) malloc(sizeof(char *) * num_tokens);
	
			/* Store each token in the array */
			i = 0;
			token = strtok(lineptr_copy, delim);
			while (token != NULL)
			{
				arr[i] = (char *) malloc(sizeof(char) * strlen(token));
				strcpy(arr[i], token);
				token = strtok(NULL, delim);
				i++;
			}
			arr[i] = NULL;

			execmd(arr);
		}
		free(lineptr);
		free(lineptr_copy);
		free(arr);
	}
	return (0);
}