#include "he.h"

int check_arg(int ac, char **av)
{
	int i = 0;
	int j;
	while (++i < ac)
	{
		j = 0;
		while (av[i][j] && ft_strncmp(av[i - 1], "echo", 5) != 0 && i > 1)
		{
			if (av[i][j] == ' ');
				return (0);
		}
		j++;		
	}
	return (1);
}


int ft_count_arg(int ac, char **av)
{
	int	i;
	int	cw;
	int	j;

	i = 0;
	cw = 0;
	while (++i <= ac)
	{
		j = 0;
		while (av[i][j] == ' ' || av[i][j] == '(' || av[i][j] == ')')
			j++;
		if (av[i][j])
			cw++;
		while (av[i][j] == ' ' || av[i][j] == '(' || av[i][j] == ')')
			j++;
	}
	return (cw);
}

void init_av(int ac, char **av, t_str_part	*str_part, int cw)
{
	int	i = 0;
	int	level = 0;
	int j;
	int check = 0;
	int n = 0;

	while (++i <= ac && n < cw)
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] == '(')
				level++;
			if (av[i][j] == ')')
				level--;
			j++;
		}
		if (ft_strncmp(av[i], "exit", 5) == 0)
		{
			str_part[n].str = "exit";
			str_part[n].type = cmd;
			str_part[n].level = level;
		}
		else if (ft_strncmp(av[i], "evn", 4) == 0)
		{
			str_part[n].str = "evn";
			str_part[n].type = cmd;
			str_part[n].level = level;
		}
		else if (ft_strncmp(av[i], "unset", 6) == 0)
		{
			str_part[n].str = "unset";
			str_part[n].type = cmd;
			str_part[n].level = level;
		}
		else if (ft_strncmp(av[i], "export", 7) == 0)
		{
			str_part[n].str = "export";
			str_part[n].type = cmd;
			str_part[n].level = level;
		}
		else if (ft_strncmp(av[i], "pwd", 4) == 0)
		{
			str_part[n].str = "pwd";
			str_part[n].type = cmd;
			str_part[n].level = level;
		}
		else if (ft_strncmp(av[i], "cd", 3) == 0)
		{
			str_part[n].str = "cd";
			str_part[n].type = cmd;
			str_part[n].level = level;
		}
		else if (ft_strncmp(av[i], "echo", 4) == 0 && ft_strncmp(av[i + 1], "-n", 3) == 0)
		{
			str_part[n].str = "echo -n";
			str_part[n].type = cmd;
			str_part[n].level = level;
		}
		else if (ft_strncmp(av[i], "echo", 4) == 0)
		{
			str_part[n].str = "echo";
			str_part[n].type = cmd;
			str_part[n].level = level;
		}
	}
	// if (level != 0)
	// 	exit(e) !!!!!!!!!!!!!!!!! - or + ()
	
}



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
