#include "he.h"


int main(int ac, char  **av)
{

	char *input;

	input = readline("minishell> ");

	











	
	t_str_part	*str_parts = malloc(ft_count_arg(ac, av) * sizeof(t_str_part));





	str_parts[0].str = "ls";
	str_parts[0].type = cmd;
	str_parts[0].level = 0;

	if (str_parts[0].type == sep)
		return (1);
	if (str_parts[str_parts_size - 1].type == sep || str_parts[str_parts_size - 1].type == red)
		return (1);

	int	i = str_parts_size - 1;
	while (i >= 0)
	{
		if (str_parts[i].type == sep && str_parts[i].level == 0)
			return (i);
		i--;
	}

	int	i = 0;
	while (i < str_parts_size)
	{
		if (str_parts[i].type == red && str_parts[i + i].level > )
		i++;
	}


	
	return 0;
}
