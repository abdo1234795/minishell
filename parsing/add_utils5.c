/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_utils5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:05:12 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/26 18:12:29 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	init_first_last_fi(t_v *v, t_sp_var *va, char **static_buffer, int l)
{
	if (!ft_isspace(v->first) && l != 1)
	{
		*static_buffer = ft_strjoin(v->buff, v->bib[v->i++], &va->allocs);
		add_expanded_token(v, &va->var->tokens, *static_buffer, va);
		*static_buffer = NULL;
	}
	else if (!ft_isspace(v->first) && l == 1 && va->var->wait_more_args)
	{
		*static_buffer = ft_strjoin(v->buff, v->bib[v->i++], &va->allocs);
		v->i = -10;
	}
	else if (!ft_isspace(v->first) && l == 1 && !va->var->wait_more_args)
	{
		*static_buffer = ft_strjoin(v->buff, v->bib[v->i++], &va->allocs);
		add_expanded_token(v, &va->var->tokens, *static_buffer, va);
		*static_buffer = NULL;
	}
	else
		add_expanded_token(v, &va->var->tokens, v->buff, va);
}

void	init_first_last(t_v *v, t_sp_var *va, char **static_buffer)
{
	int	l;

	v->expanded_value = expand_env_vars(v->new_buff, va);
	if (ft_strcmp(v->expanded_value, "") != 0)
	{
		v->first = v->expanded_value[0];
		v->last = v->expanded_value[ft_strlen(v->expanded_value) - 1];
	}
	v->bib = ft_split(v->expanded_value, ' ', &va->allocs);
	v->i = 0;
	l = 0;
	while (v->bib[l])
		l++;
	init_first_last_fi(v, va, static_buffer, l);
	v->s = v->i;
}

void	init_first_last_ambig(t_v *v, t_sp_var *va, char **static_buffer)
{
	(void)static_buffer;
	v->expanded_value = expand_env_vars(v->new_buff, va);
	v->first = '\0';
	v->last = '\0';
	if (ft_strcmp(v->expanded_value, "") != 0
		&& ft_strlen(v->expanded_value) >= 1)
	{
		v->first = v->expanded_value[0];
		v->last = v->expanded_value[ft_strlen(v->expanded_value) - 1];
	}
	v->bib = ft_split(v->expanded_value, ' ', &va->allocs);
	while (v->bib[v->i])
		v->i++;
	if (v->i > 1)
		va->st_ambiguous = true;
}

void	p_ex_with_buffer_1_fi(t_v *v, t_sp_var *va, char **static_buffer)
{
	if (v->i != -10)
	{
		while (v->bib[v->i])
			v->i++;
		v->t = v->i - 1;
		v->i = v->s;
		while (v->bib[v->i])
		{
			if (v->i != v->t)
				add_expanded_token(v, &va->var->tokens, v->bib[v->i++], va);
			else
				break ;
		}
		if (ft_isspace(v->last) && v->t != 0)
			add_expanded_token(v, &va->var->tokens, v->bib[v->t], va);
		else if (v->t != 0)
			*static_buffer = v->bib[v->t];
	}
}

void	p_ex_with_buffer_1(t_v *v, t_sp_var *va, char **static_buffer)
{
	init_first_last(v, va, static_buffer);
	if (v->ambiguous)
		va->st_ambiguous = false;
	if (va->var->wait_more_args)
		p_ex_with_buffer_1_fi(v, va, static_buffer);
	else
	{
		while (v->bib[v->i])
			add_expanded_token(v, &va->var->tokens, v->bib[v->i++], va);
		*static_buffer = NULL;
	}
}
