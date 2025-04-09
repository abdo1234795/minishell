#include "he.h"

int f1_parenthesis(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '(')
			j++;
		if (str[i] == ')')
			j--;
		i++;
	}
	if (j != 0)
		return (0);
	return (1);
}

int f2_unclosed_Dquotes(char *str)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (str[++i])
	{
		if (str[i] == '\"')
			j++;
	}
	if (j % 2 != 0)
		return (0);
	return (1);
}

int f3_unclosed_quotes(char *str)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (str[++i])
	{
		if (str[i] == '\'')
			j++;
	}
	if (j % 2 != 0)
		return (0);
	return (1);
}

char *new_input(char *str)
{
	if (!str)
		return (NULL);
	while (*str == ' ')
		str++;
	if (*str == '(' && *(str + 1) && *(str + 1) == ')')
		str = str + 2;
	return (str);
}

int check_input(char* input)
{
	if (!f1_parenthesis(input))//number of parenthesis is no the same
		return (0);
	if (!f2_unclosed_Dquotes(input))//number of " is no the same
		return (0);
	if (!f2_unclosed_quotes(input))//number of ' is no the same
		return (0);
	
}