/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:14:00 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/26 18:22:35 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*get_var_value(char *var_name, t_env *env, t_sp_var *va)
{
	char	*env_value;

	if (!var_name)
		return (NULL);
	env_value = get_key_value(var_name, env);
	if (env_value)
		return (ft_strdup(env_value, &va->allocs, P_GARBAGE));
	return (ft_strdup("", &va->allocs, P_GARBAGE));
}

int	init_expand_var(t_expand *e, t_sp_var *va)
{
	if (!e)
		return (0);
	e->i = 0;
	e->j = 0;
	e->tmp = NULL;
	e->m = 0;
	e->k = 0;
	e->var_name = NULL;
	e->value = NULL;
	e->result = mmallocc(9999, &va->allocs, P_GARBAGE);
	if (!e->result)
		return (0);
	return (1);
}

int	dollar(char *str, t_expand *e)
{
	if ((str[e->i + 1] < '0' || (str[e->i + 1] > '9' && str[e->i + 1] < 'A')
			|| (str[e->i + 1] > 'Z' && str[e->i + 1] < '_')
			|| (str[e->i + 1] > '_' && str[e->i + 1] < 'a')
			|| str[e->i + 1] > 'z')
		&& str[e->i + 1] != '$' && str[e->i + 1] != '?')
	{
		e->i++;
		e->result[e->j++] = '$';
		return (1);
	}
	return (0);
}

int	expand_part0(char *str, t_sp_var *va, t_expand *e)
{
	int	a;

	a = -1;
	if (dollar(str, e))
		return (1);
	e->i++;
	a = expand_part1(str, va, e);
	if (a == 1)
		return (1);
	else if (a == 0)
		return (0);
	if (str[e->i] == '$')
		return (1);
	a = expand_part2(str, va, e);
	if (a == 0)
		return (0);
	else if (a == 1)
		return (1);
	return (3);
}

char	*expand_env_vars(char *str, t_sp_var *va)
{
	t_expand	*e;
	int			b;

	e = mmallocc(sizeof(t_expand), &va->allocs, P_GARBAGE);
	if (!str || !e)
		return (NULL);
	if (!init_expand_var(e, va))
		return (NULL);
	while (str[e->i])
	{
		if (str[e->i] == '$' && str[e->i + 1]
			&& va->var->state != SINGLE_QUOTED)
		{
			b = expand_part0(str, va, e);
			if (b == 1)
				continue ;
			else if (b == 0)
				return (NULL);
		}
		else
			e->result[e->j++] = str[e->i++];
	}
	e->result[e->j] = '\0';
	return (ft_strdup(e->result, &va->allocs, P_GARBAGE));
}
