/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_utils1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:20:27 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/30 09:23:43 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	expand_part1(char *str, t_sp_var *va, t_expand *e)
{
	if (str[e->i] == '?')
	{
		e->m = 0;
		if (g_signal_pid == 0)
			va->status = va->status;
		else if (g_signal_pid == -1)
		{
			va->status = 1;
			g_signal_pid = 0;
		}
		e->tmp = ft_itoa(va->status, va);
		if (!e->tmp)
			return (0);
		while (e->tmp[e->m] != '\0')
			e->result[e->j++] = e->tmp[e->m++];
		e->i++;
		return (1);
	}
	return (3);
}

int	expand_part2(char *str, t_sp_var *va, t_expand *e)
{
	e->var_name = mmallocc(256, &va->allocs, P_GARBAGE);
	if (!e->var_name)
		return (0);
	e->k = 0;
	if (str[e->i] && (str[e->i] >= '0' && str[e->i] <= '9'))
	{
		e->i++;
		return (1);
	}
	while (str[e->i] && ((str[e->i] >= 'a' && str[e->i] <= 'z')
			|| (str[e->i] >= 'A' && str[e->i] <= 'Z')
			|| (str[e->i] >= '0' && str[e->i] <= '9')
			|| str[e->i] == '_'))
		e->var_name[e->k++] = str[e->i++];
	e->var_name[e->k] = '\0';
	e->value = get_var_value(e->var_name, va->env, va);
	if (e->value)
	{
		e->m = 0;
		while (e->value[e->m])
			e->result[e->j++] = e->value[e->m++];
	}
	return (3);
}
