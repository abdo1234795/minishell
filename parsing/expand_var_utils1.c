/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_utils1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:20:27 by abel-had          #+#    #+#             */
/*   Updated: 2025/06/19 09:01:57 by abel-had         ###   ########.fr       */
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

static int	var_name_condition(char *str, t_expand *e)
{
	return (str[e->i] && ((str[e->i] >= 'a' && str[e->i] <= 'z')
			|| (str[e->i] >= 'A' && str[e->i] <= 'Z')
			|| (str[e->i] >= '0' && str[e->i] <= '9')
			|| str[e->i] == '_'));
}

int	expand_part2(char *str, t_sp_var *va, t_expand *e)
{
	size_t	val_len;

	e->var_name = mmallocc(256, &va->allocs, P_GARBAGE);
	if (!e->var_name)
		return (0);
	e->k = 0;
	if (str[e->i] && (str[e->i] >= '0' && str[e->i] <= '9'))
	{
		e->i++;
		return (1);
	}
	while (var_name_condition(str, e))
		e->var_name[e->k++] = str[e->i++];
	e->var_name[e->k] = '\0';
	e->value = get_var_value(e->var_name, va->env, va);
	if (e->value)
	{
		val_len = ft_strlen(e->value);
		if (!resize_expand_buffer(e, va, val_len + 1))
			return (0);
		e->m = 0;
		while (e->value[e->m])
			e->result[e->j++] = e->value[e->m++];
	}
	return (3);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*d;
	unsigned char	*s;

	i = 0;
	if (src == dst)
		return (dst);
	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	if (!src && !dst)
		return (NULL);
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

int	resize_expand_buffer(t_expand *e, t_sp_var *va, size_t needed)
{
	char	*new_buf;
	size_t	new_capacity;

	if (e->j + needed < e->capacity)
		return (1);
	new_capacity = e->capacity * 2;
	while (new_capacity < e->j + needed)
		new_capacity *= 2;
	new_buf = mmallocc(new_capacity, &va->allocs, P_GARBAGE);
	if (!new_buf)
		return (0);
	ft_memcpy(new_buf, e->result, e->j);
	e->result = new_buf;
	e->capacity = new_capacity;
	return (1);
}
