/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:55:35 by aulicna           #+#    #+#             */
/*   Updated: 2023/11/21 12:16:21 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simple_shell.h"

int	main(int argc, char **argv)
{
	char	*input;

	(void) argc;
	(void) argv;
	input = readline("simple-shell\% ");
	printf("%s\n", input);
	free(input);
	return(0);

	/* Original simple shell code
	char	*prompt;
	char	*lineptr;
	size_t	n;

	(void) argc;
	(void) argv;
	prompt = "simple-shell\% ";
	printf("%s", prompt);
	n = 0;
	lineptr = NULL;
	getline(&lineptr, &n, stdin);
	printf("%s\n", lineptr);
	free(lineptr);
	return (0);*/
}