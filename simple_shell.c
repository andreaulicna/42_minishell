/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_shell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:55:35 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/22 14:36:56 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/minishell.h"
#include "libftprintf/ft_printf.h"

char	*get_location(char *command)
{
	char	*path;
	char	*path_copy;
	int	command_length;
	char	*path_token;
	int	directory_lenght;
	char	*file_path;
	struct stat	buffer;

	path = getenv("PATH");
	if (path)
	{
		// a copy is needed bcs strtok destroyes the original string
		path_copy = strdup(path);

		/* Get the length of the command */
		command_length = strlen(command);

		/* Break down the path_copy variable into individual tokens */
		path_token = strtok(path_copy, ":");

		/* Run a while loop to get and test the exact path for the command */
		while (path_token != NULL)
		{
			directory_lenght = strlen(path_token);
			file_path = malloc(command_length + directory_lenght + 2);
			strcpy(file_path, path_token);
			strcat(file_path, "/");
			strcat(file_path, command);
			strcat(file_path, "\0");

			if (stat(file_path, &buffer) == 0)
			{
				free(path_copy);
				return (file_path);
			}
			else
			{
				free(file_path);
				path_token = strtok(NULL, ":");
			}
		}
		free(path_copy);
		if (stat(command, &buffer) == 0)
			return (command);
		return (NULL);
	}
	return (NULL);
}

void execmd(char **arr)
{
	char	*command;
	char	*actual_command;

	if (arr != NULL)
	{
		command = arr[0];
		actual_command = get_location(command);
		if (execve(actual_command, arr, NULL) == -1)
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