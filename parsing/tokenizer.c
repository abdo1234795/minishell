/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:17:11 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/27 11:17:36 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_token	*fill_tokenize(t_var *v)
{
	bool	has_syntax_error;

	has_syntax_error = fill_tken_part1(v);
	if (has_syntax_error && v->has_heredoc)
		return (fill_tken_part2(v));
	if (has_syntax_error && !v->has_heredoc)
	{
		return (fill_tken_part3(v));
	}
	if (v->state != UNQUOTED)
	{
		print_error("minishell: unexpected EOF while looking for matching `");
		if (v->state == SINGLE_QUOTED)
			print_error("''\n");
		else
			print_error("\"'\n");
		return (NULL);
	}
	return (v->tokens);
}

t_token	*tokenize_input(t_sp_var *va)
{
	va->var = mmallocc(sizeof(t_var), &va->allocs, P_GARBAGE);
	init_var(va);
	while (va->line[++va->var->j])
	{
		va->var->c = va->line[va->var->j];
		va->var->a = while_part1(va);
		if (va->var->a != 2)
		{
			if (va->var->a == 0)
				break ;
		}
		else if (va->var->state == UNQUOTED && ft_isspace(va->var->c))
		{
			if (!fifth_condition(va))
				break ;
			continue ;
		}
		else
			va->var->buffer[va->var->i++] = va->var->c;
	}
	sixth_condition(va);
	return (fill_tokenize(va->var));
}
