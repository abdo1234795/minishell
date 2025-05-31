/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_utils6.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:12:46 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/26 18:13:42 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	ambiguous_1(t_v *v, t_sp_var *va, char **static_buffer)
{
	if (va->var->wait_more_args && va->st_ambiguous)
	{
		*static_buffer = ft_strjoin(v->buff, v->new_buff, &va->allocs);
		return (1);
	}
	else if (va->var->wait_more_args && v->i == 0)
	{
		*static_buffer = ft_strjoin(v->buff, "", &va->allocs);
		return (1);
	}
	return (0);
}

int	ambiguous_2(t_v *v, t_sp_var *va, char **static_buffer)
{
	if (va->var->wait_more_args && !va->st_ambiguous)
	{
		if (!ft_isspace(v->first) && !ft_isspace(v->last))
			*static_buffer = ft_strjoin(v->buff, v->bib[0], &va->allocs);
		else if (ft_isspace(v->first) && ft_isspace(v->last))
		{
			add_expanded_token(v, &va->var->tokens, v->buff, va);
			add_expanded_token(v, &va->var->tokens, v->bib[0], va);
			*static_buffer = NULL;
		}
		else if (!ft_isspace(v->first) && ft_isspace(v->last))
		{
			*static_buffer = ft_strjoin(v->buff, v->bib[0], &va->allocs);
			add_expanded_token(v, &va->var->tokens, *static_buffer, va);
			*static_buffer = NULL;
		}
		else if (ft_isspace(v->first) && !ft_isspace(v->last))
		{
			add_expanded_token(v, &va->var->tokens, *static_buffer, va);
			*static_buffer = v->buff;
		}
		return (1);
	}
	return (0);
}

int	ambiguous_3(t_v *v, t_sp_var *va, char **static_buffer)
{
	if (!va->var->wait_more_args && v->i == 0)
	{
		add_token(v, &va->var->tokens);
		return (1);
	}
	else if (!va->var->wait_more_args && va->st_ambiguous)
	{
		*static_buffer = ft_strjoin(v->buff, v->new_buff, &va->allocs);
		v->buff = *static_buffer;
		if (va->st_ambiguous)
			v->ambiguous = true;
		va->st_ambiguous = false;
		add_anbiguous_token(v, &va->var->tokens);
		return (1);
	}
	return (0);
}

int	ambiguous_4(t_v *v, t_sp_var *va, char **static_buffer)
{
	if (!va->var->wait_more_args && !va->st_ambiguous)
	{
		if (ft_isspace(v->first))
		{
			add_expanded_token(v, &va->var->tokens, v->buff, va);
			add_expanded_token(v, &va->var->tokens, v->bib[0], va);
			*static_buffer = NULL;
		}
		else
		{
			*static_buffer = ft_strjoin(v->buff, v->bib[0], &va->allocs);
			add_expanded_token(v, &va->var->tokens, *static_buffer, va);
			*static_buffer = NULL;
		}
		return (1);
	}
	return (0);
}

int	ambiguous_0(t_v *v, t_sp_var *va, char **static_buffer)
{
	if (should_expand_token(v, va) && va->st_ambiguous_red
		&& va->var->state == UNQUOTED)
	{
		init_first_last_ambig(v, va, static_buffer);
		if (ambiguous_1(v, va, static_buffer))
			return (1);
		else if (ambiguous_2(v, va, static_buffer))
			return (1);
		else if (ambiguous_3(v, va, static_buffer))
			return (1);
		else if (ambiguous_4(v, va, static_buffer))
			return (1);
	}
	return (0);
}
