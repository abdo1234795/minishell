/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:58:16 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/27 11:21:44 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	init_last_ambig(t_v *v, t_sp_var *va, int a)
{
	v->expanded_value = expand_env_vars(v->new_buff, va);
	if (ft_strcmp(v->expanded_value, "") != 0
		&& ft_strlen(v->expanded_value) >= 1)
		v->last = v->expanded_value[ft_strlen(v->expanded_value) - 1];
	else if (ft_strlen(v->expanded_value) <= 0)
		v->last = '\0';
	v->bib = ft_split(v->expanded_value, ' ', &va->allocs);
	if (a == 1)
	{
		while (v->bib[v->i])
			v->i++;
		if (v->i > 1)
			va->st_ambiguous = true;
	}
}

int	ambiguous_nob_1(t_v *v, t_sp_var *va, char **static_buffer)
{
	if (va->var->wait_more_args && va->st_ambiguous)
	{
		*static_buffer = ft_strdup(v->new_buff, &va->allocs, P_GARBAGE);
		return (1);
	}
	else if (va->var->wait_more_args && v->i == 0)
	{
		*static_buffer = ft_strdup("", &va->allocs, P_GARBAGE);
		return (1);
	}
	else if (va->var->wait_more_args && !va->st_ambiguous && v->i == 1)
	{
		if (ft_isspace(v->last))
		{
			add_expanded_token(v, &va->var->tokens, v->bib[0], va);
			*static_buffer = NULL;
		}
		else
			*static_buffer = ft_strdup(v->bib[0], &va->allocs, P_GARBAGE);
		return (1);
	}
	return (0);
}

int	ambiguous_nob_2(t_v *v, t_sp_var *va, char **static_buffer)
{
	if (!va->var->wait_more_args && va->st_ambiguous)
	{
		v->buff = v->new_buff;
		if (va->st_ambiguous)
			v->ambiguous = true;
		va->st_ambiguous = false;
		add_anbiguous_token(v, &va->var->tokens);
		return (1);
	}
	else if (!va->var->wait_more_args && !va->st_ambiguous && v->i == 0)
	{
		v->buff = v->new_buff;
		add_anbiguous_token(v, &va->var->tokens);
		return (1);
	}
	else if (!va->var->wait_more_args && !va->st_ambiguous && v->i == 1)
	{
		add_expanded_token(v, &va->var->tokens, v->bib[0], va);
		*static_buffer = NULL;
		return (1);
	}
	return (0);
}

void	wait_or_add(t_v *v, t_sp_var *va, char **static_buffer)
{
	if (va->var->wait_more_args)
		p_ex_without_buffer_fill(v, va, static_buffer);
	else
	{
		v->s = -1;
		while (v->bib[++v->s])
			add_expanded_token(v, &va->var->tokens, v->bib[v->s], va);
		*static_buffer = NULL;
	}
}

void	p_without_buffer_last_part(t_v *v, t_sp_var *va, char **static_buffer)
{
	if (va->var->wait_more_args)
		*static_buffer = ft_strdup(v->new_buff, &va->allocs, P_GARBAGE);
	else
	{
		if (!(va->st_quote))
			v->new_token->need_expand = true;
		v->buff = v->new_buff;
		add_token(v, &va->var->tokens);
		v->buff = NULL;
		*static_buffer = NULL;
		va->st_quote = false;
	}
}
