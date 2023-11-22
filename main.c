/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:55:35 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/22 11:13:07 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simple_shell.h"

int	main(int argc, char **argv)
{
	char	*input;
	(void) argc;
	(void) argv;
	while (1)
	{
		input = readline("simple-shell\% ");
		if (input == NULL)
		{
			printf("exit");
			//free(input);
			return (-1);
		}
		printf("%s\n\n", input);
		free(input);
	}
// check unsigned printf with -1 
//	ft_printf("my printf: %u\n", -1);
//	printf("printf: %u\n", -1);
	return(0);
}



////Original simple shell code
////-> uses getline which is not allowed for minishell
//
//int	main(int argc, char **argv)
//{
//
//	char	*prompt;
//	char	*lineptr;
//	size_t	n;
//	__ssize_t	nchars_read;
//	char	*lineptr_copy;
//	const char	*delim = " \n";
//	char	*token;
//	int	num_tokens;
//	char	**arr;
//	int	i;
//
//	(void) argc;
//	(void) argv;
//	prompt = "simple-shell\% ";
//	printf("%s", prompt);
//	n = 0;
//	lineptr = NULL;
//	nchars_read = getline(&lineptr, &n, stdin);
//	/* Dynamically allocate space to store a copy of the string read */
//	lineptr_copy = (char *) malloc(sizeof(char) * nchars_read);
//	if (lineptr_copy == NULL)
//	{
//		perror("tsh: memory allocation error");
//		return (-1);
//	}
//	/* Make a copy of the lineptr */
//	strcpy(lineptr_copy, lineptr);
//	if (nchars_read == -1)
//	{
//		printf("exit\n");
//		free(lineptr);
//		free(lineptr_copy);
//		return (-1);
//	}
//	else
//	{
//		
//		/* Calculate the total number of tokens expected from the string */
//		/* split the string (lineptr) into an array of words */
//		token = strtok(lineptr, delim);
//		
//		/* determine how many tokens are there*/
//		num_tokens = 0;
//		while (token != NULL)
//		{
//			token = strtok(NULL, delim);
//			num_tokens++;
//		}
//		num_tokens++;
//	
//		/* Allocate space to hold the array of strings (tokens) */
//		arr = (char **) malloc(sizeof(char *) * num_tokens);
//	
//		/* Store each token in the array */
//		i = 0;
//		token = strtok(lineptr_copy, delim);
//		while (token != NULL)
//		{
//			arr[i] = (char *) malloc(sizeof(char) * strlen(token));
//			strcpy(arr[i], token);
//			printf("%s\n", arr[i]);
//			token = strtok(NULL, delim);
//			i++;
//		}
//		arr[i] = NULL;
//		
//		free(lineptr);
//		free(lineptr_copy);
//		i = 0;
//		while(arr[i])
//		{
//			free(arr[i]);
//			i++;
//		}
//		free(arr);
//	}
//	return (0);
//}