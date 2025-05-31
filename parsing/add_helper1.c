/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_helper1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:44:22 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/26 17:48:19 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	ft_fill_1(t_sp_var *va)
{
	if (va->var->buffer[0] == '>' || va->var->buffer[0] == '<'
		|| va->var->buffer[0] == '|')
	{
		if (!check_operator_validity(va->var->buffer, &va->var->state))
		{
			add_error_token(&va->var->tokens, va);
			return (0);
		}
	}
	return (1);
}

int	ft_fill_2(t_v *v, t_sp_var *va)
{
	if (va->var->state == DOUBLE_QUOTED)
		v->new_buff = remove_character(va, '\"');
	else if (va->var->state == SINGLE_QUOTED)
		v->new_buff = remove_character(va, '\'');
	else if (va->var->state == UNQUOTED)
		v->new_buff = ft_strdup(va->var->buffer, &va->allocs, P_GARBAGE);
	if (!v->new_buff)
		return (0);
	return (1);
}

void	init_variable(t_v *v, t_token *tokens)
{
	if (!v)
		return ;
	v->new_token = NULL;
	v->current = NULL;
	v->prev_token = last_token(tokens);
	v->buff = NULL;
	v->joined = NULL;
	v->quote = false;
	v->new_buff = NULL;
	v->first = '\0';
	v->last = '\0';
	v->tmp = NULL;
	v->expanded_value = NULL;
	v->bib = NULL;
	v->i = 0;
	v->s = 0;
	v->t = 0;
	v->ambiguous = false;
}

t_token	*create_new_token(t_token **new_token, char *new_buff, t_sp_var *va)
{
	*new_token = mmallocc(sizeof(t_token), &va->allocs, P_GARBAGE);
	if (!*new_token)
		return (NULL);
	(*new_token)->value = new_buff;
	(*new_token)->type = text;
	(*new_token)->syn_err = false;
	(*new_token)->heredoc = false;
	(*new_token)->need_expand = false;
	(*new_token)->newline = false;
	(*new_token)->wait_more_args = false;
	(*new_token)->next = NULL;
	(*new_token)->ambiguous = false;
	return (*new_token);
}

int	need_expandd(char *str, t_quote_state *state)
{
	int	i;

	i = 0;
	if (*state != SINGLE_QUOTED)
	{
		while (str[i])
		{
			if (str[i] == '$')
				return (1);
			i++;
		}
	}
	return (0);
}
