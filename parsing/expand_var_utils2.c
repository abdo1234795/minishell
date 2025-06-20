/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 08:51:29 by abel-had          #+#    #+#             */
/*   Updated: 2025/06/19 08:54:22 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	process_dollar(char *str, t_sp_var *va, t_expand *e, int *b)
{
	if (!resize_expand_buffer(e, va, 1))
		return (0);
	*b = expand_part0(str, va, e);
	if (*b == 0)
		return (0);
	return (1);
}

char	*handle_expansion(char *str, t_sp_var *va, t_expand *e)
{
	int	b;

	while (str[e->i])
	{
		if (str[e->i] == '$' && str[e->i + 1]
			&& va->var->state != SINGLE_QUOTED)
		{
			if (!process_dollar(str, va, e, &b))
				return (NULL);
			if (b == 1)
				continue ;
		}
		else
		{
			if (!resize_expand_buffer(e, va, 1))
				return (NULL);
			e->result[e->j++] = str[e->i++];
		}
	}
	e->result[e->j] = '\0';
	return (ft_strdup(e->result, &va->allocs, P_GARBAGE));
}
