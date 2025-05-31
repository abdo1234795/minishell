/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_itoa.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:33:42 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/27 11:37:21 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_digit_count(long int i)
{
	int	count;

	count = 0;
	if (i == 0)
		return (1);
	if (i < 0)
	{
		count++;
		i = -i;
	}
	while (i > 0)
	{
		count++;
		i /= 10;
	}
	return (count);
}

static void	fill_number(char *str, long int nb, int len)
{
	int	i;

	if (nb == 0)
		str[0] = '0';
	else
	{
		if (nb < 0)
		{
			str[0] = '-';
			nb = -nb;
		}
		i = len - 1;
		while (nb > 0)
		{
			str[i--] = (nb % 10) + '0';
			nb /= 10;
		}
	}
}

char	*ft_itoa(int n, t_sp_var *va)
{
	long int	nb;
	int			len;
	char		*str;

	nb = n;
	len = ft_digit_count(nb);
	str = mmallocc(len + 1, &va->allocs, P_GARBAGE);
	if (!str)
		return (NULL);
	str[len] = '\0';
	fill_number(str, nb, len);
	return (str);
}
