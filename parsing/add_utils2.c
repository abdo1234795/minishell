/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:55:12 by abel-had          #+#    #+#             */
/*   Updated: 2025/06/02 17:51:02 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_v	*init_token_vars(t_sp_var *va, bool *quote)
{
	t_v	*v;

	v = mmallocc(sizeof(t_v), &va->allocs, P_GARBAGE);
	if (!v)
		return (NULL);
	init_variable(v, va->var->tokens);
	update_quote_state(va, quote);
	v->quote = *quote;
	return (v);
}

int	prepare_token(t_sp_var *va, t_v *v)
{
	if (!ft_fill_1(va))
		return (0);
	if (!ft_fill_2(v, va))
		return (0);
	v->new_token = create_new_token(&v->new_token, v->new_buff, va);
	if (!v->new_token)
		return (0);
	v->new_token->ambiguous = false;
	v->new_token->need_expand = false;
	red_pip_txt(v, &va->var->state);
	return (1);
}

int	while_ambiguous_part(t_sp_var *va, int *x)
{
	if (va->vpt->current->next && va->vpt->current->next->ambiguous && *x == 0)
	{
		if (va->vpt->current_cmd)
		{
			va->vpt->current_cmd->name = NULL;
		}
		va->vpt->current_cmd->am = true;
		print_error("minishell: ambiguous redirect\n");
		*x = 1;
		va->status = 1;
	}
	va->vpt->result = while_part_if_2(va, x);
	if (va->vpt->result == 0)
		return (0);
	if (va->vpt->result == 2)
		return (2);
	if (va->vpt->result == 10)
		return (10);
	while_part_print_err(va, x);
	return (1);
}

int	while_part_fiill(t_sp_var *va, int *x)
{
	int	a;

	if (va->vpt->current->type == pip)
	{
		if (!while_part_if_1(va, x))
			return (0);
		*x = 0;
		va->vpt->tmp_err = 0;
		va->vpt->err = 0;
	}
	else if (va->vpt->current->type == red)
	{
		a = while_ambiguous_part(va, x);
		if (a != 1)
			return (a);
	}
	else if (va->vpt->current->type == text)
		add_argument(va->vpt->current_cmd, va->vpt->current->value, va);
	if (va->vpt->current)
		va->vpt->current = va->vpt->current->next;
	return (1);
}
