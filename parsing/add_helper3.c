/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_helper3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:48:25 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/30 11:13:34 by abel-had         ###   ########.fr       */
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
	return (!(v->prev_token && ft_strcmp(v->prev_token->value, "<<") == 0)
		&& need_expandd(v->new_buff, &va->var->state));
}
