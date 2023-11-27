#include "../inc/minishell.h"

int	ft_echo(char **args)
{
	int	n_flag;
	int	i;

	n_flag = 0;
	i = 1;
	if (strs_count(args) > 1)
	{
		if (args[1] && ft_strncmp(args[1], "-n", 2) == 0)
		{
			n_flag = 1;
			i++;
		}
		while (args[i] != NULL)
		{
			ft_printf("%s", args[i]);
			if (args[i + 1] && args[i][0] != '\0')
				ft_printf(" ");
			i++;
		}
	}
	if (n_flag == 0)
		ft_printf("\n");
	return (0);
}