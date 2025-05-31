/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:53:33 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/27 11:21:26 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	p_ex_with_buffer_2(t_v *v, t_sp_var *va, char **static_buffer)
{
	if (should_expand_token(v, va) && va->var->state == DOUBLE_QUOTED)
	{
		v->expanded_value = expand_env_vars(v->new_buff, va);
		if (va->var->wait_more_args)
			*static_buffer = ft_strjoin(v->buff, v->expanded_value,
					&va->allocs);
		else
		{
			*static_buffer = ft_strjoin(v->buff, v->expanded_value,
					&va->allocs);
			add_expanded_token(v, &va->var->tokens, *static_buffer, va);
			*static_buffer = NULL;
		}
		return (1);
	}
	return (0);
}

void	p_no_ex_with_buffer(t_v *v, t_sp_var *va, char **static_buffer)
{
	if (va->var->wait_more_args)
		*static_buffer = ft_strjoin(v->buff, v->new_buff, &va->allocs);
	else
	{
		if (!(va->st_quote))
			v->new_token->need_expand = true;
		if (v->buff)
			*static_buffer = ft_strjoin(v->buff, v->new_buff, &va->allocs);
		v->buff = *static_buffer;
		if (va->st_ambiguous)
			v->ambiguous = true;
		va->st_ambiguous = false;
		add_token(v, &va->var->tokens);
		*static_buffer = NULL;
		va->st_quote = false;
	}
}

int	p_with_buffer(t_v *v, t_sp_var *va, char **static_buffer)
{
	if (ambiguous_0(v, va, static_buffer))
		return (1);
	else if (should_expand_token(v, va) && va->var->state != DOUBLE_QUOTED)
		p_ex_with_buffer_1(v, va, static_buffer);
	else if (p_ex_with_buffer_2(v, va, static_buffer))
		return (1);
	else
		p_no_ex_with_buffer(v, va, static_buffer);
	return (0);
}

void	update_quote_state(t_sp_var *va, bool *QUOTE)
{
	if (va->var->state != UNQUOTED && (!(*QUOTE) || (*QUOTE)))
		*QUOTE = true;
	else if (va->var->state == UNQUOTED && !(*QUOTE))
		*QUOTE = false;
	else if (va->var->state == UNQUOTED && (*QUOTE))
		*QUOTE = true;
}
