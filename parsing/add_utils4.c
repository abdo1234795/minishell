/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_utils4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:59:45 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/26 18:04:59 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	p_ex_without_buffer_fill(t_v *v, t_sp_var *va, char **static_buffer)
{
	if (!v->bib || !v->bib[0])
	{
		*static_buffer = NULL;
		return ;
	}
	v->i = 0;
	while (v->bib[v->i])
		v->i++;
	v->t = v->i - 1;
	v->i = 0;
	while (v->bib[v->i])
	{
		if (v->i != v->t)
			add_expanded_token(v, &va->var->tokens, v->bib[v->i++], va);
		else
			break ;
	}
	if (ft_isspace(v->last))
		add_expanded_token(v, &va->var->tokens, v->bib[v->t], va);
	else
		*static_buffer = v->bib[v->t];
}

void	p_ex_without_buffer(t_v *v, t_sp_var *va, char **static_buffer)
{
	v->expanded_value = expand_env_vars(v->new_buff, va);
	if (ft_strcmp(v->expanded_value, "") != 0
		&& ft_strlen(v->expanded_value) >= 1)
		v->last = v->expanded_value[ft_strlen(v->expanded_value) - 1];
	else if (ft_strlen(v->expanded_value) <= 0)
		v->last = '\0';
	v->bib = ft_split(v->expanded_value, ' ', &va->allocs);
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

void	p_without_buffer(t_v *v, t_sp_var *va, char **static_buffer)
{
	if (should_expand_token(v, va) && va->st_ambiguous_red
		&& va->var->state == UNQUOTED)
	{
		init_last_ambig(v, va, 1);
		if (ambiguous_nob_1(v, va, static_buffer))
			return ;
		else if (ambiguous_nob_2(v, va, static_buffer))
			return ;
	}
	else if (should_expand_token(v, va) && va->var->state != DOUBLE_QUOTED)
		p_without_buffer_f(v, va, static_buffer);
	else if (should_expand_token(v, va) && va->var->state == DOUBLE_QUOTED)
	{
		v->expanded_value = expand_env_vars(v->new_buff, va);
		if (va->var->wait_more_args)
			*static_buffer = ft_strdup(v->expanded_value,
					&va->allocs, P_GARBAGE);
		else
			add_expanded_token(v, &va->var->tokens, v->expanded_value, va);
		return ;
	}
	else
		p_without_buffer_last_part(v, va, static_buffer);
}

bool	ft_ambiguous_red(t_v *v)
{
	if (v->prev_token && v->prev_token->value)
	{
		if (ft_strcmp(v->prev_token->value, ">") == 0
			|| ft_strcmp(v->prev_token->value, "<") == 0
			||ft_strcmp(v->prev_token->value, ">>") == 0)
		{
			return (true);
		}
	}
	return (false);
}

void	add_anbiguous_token(t_v *v, t_token **tokens)
{
	v->new_token->value = v->buff;
	v->new_token->syn_err = false;
	v->new_token->ambiguous = true;
	if (*tokens == NULL)
		*tokens = v->new_token;
	else
	{
		v->current = *tokens;
		while (v->current->next)
			v->current = v->current->next;
		v->current->next = v->new_token;
	}
}
