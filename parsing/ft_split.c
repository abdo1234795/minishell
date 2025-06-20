#include "parsing.h"

int	ft_is_sep(char c, char *sep)
{
	int	i;

	i = 0;
	while (sep[i])
	{
		if (c == sep[i])
			return (1);
		i++;
	}
	return (0);
}

void	skip_seps(char *str, char *sep, int *i)
{
	while (ft_is_sep(str[*i], sep))
		(*i)++;
}

int	ft_words(char *str, char *sep)
{
	int	i;
	int	t;

	i = 0;
	t = 0;
	skip_seps(str, sep, &i);
	while (str[i])
	{
		if (ft_is_sep(str[i], sep))
		{
			skip_seps(str, sep, &i);
			t++;
		}
		else
			i++;
	}
	if (ft_is_sep(str[i - 1], sep) == 0 && str[i - 1])
		t++;
	return (t);
}

char	*ft_get_next(char *str, char *sep, int *i, t_sp_var *va)
{
	int		word_len;
	int		j;
	char	*s;

	word_len = 0;
	while (!ft_is_sep(str[*i + word_len], sep) && str[*i + word_len])
		word_len++;
	s = (char *)mmallocc(word_len +1, &va->allocs, P_GARBAGE);
	if (!s)
		return (NULL);
	j = 0;
	while (j < word_len)
		s[j++] = str[(*i)++];
	s[j] = '\0';
	return (s);
}

char	**ft_spplit(char *str, char *charset, t_sp_var *va)
{
	int		i;
	int		j;
	int		word_len;
	char	**arr;

	word_len = ft_words(str, charset);
	arr = (char **) mmallocc((word_len + 1) * sizeof(char **), &va->allocs, P_GARBAGE);
	if (!arr)
		return (NULL);
	j = 0;
	i = 0;
	while (j < word_len && str[i])
	{
		if (ft_is_sep(str[i], charset))
			skip_seps(str, charset, &i);
		else
			arr[j++] = ft_get_next(str, charset, &i, va);
	}
	arr[j] = NULL;
	return (arr);
}