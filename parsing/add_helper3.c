/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_helper3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:48:25 by abel-had          #+#    #+#             */
/*   Updated: 2025/06/18 18:38:41 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_token	*last_token(t_token *token)
{
	t_token	*curr;

	if (!token)
		return (NULL);
	else
	{
		curr = token;
		while (curr->next)
			curr = curr->next;
		return (curr);
	}
}

void	p_without_buffer_f(t_v *v, t_sp_var *va, char **static_buffer)
{
	init_last_ambig(v, va, 0);
	wait_or_add (v, va, static_buffer);
}

bool	should_expand_token(t_v *v, t_sp_var *va)
{
	if (!v->prev_token || !v->prev_token->value)
		return (false);
	return (!(v->prev_token && ft_strcmp(v->prev_token->value, "<<") == 0)
		&& need_expandd(v->new_buff, &va->var->state));
}

int	p_without_buffer_dq_condition(t_v *v, t_sp_var *va)
{
	return (((should_expand_token(v, va)
				|| need_expandd(v->new_buff, &va->var->state))
			&& va->var->state == DOUBLE_QUOTED));
}
