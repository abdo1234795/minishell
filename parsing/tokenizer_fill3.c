/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_fill3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:07:05 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/27 11:16:36 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	fill3_fourth_fil(t_sp_var *va)
{
	va->var->buffer[va->var->i] = '\0';
	if (!add_token_with_type(va))
	{
		va->var->i = 0;
		return (0);
	}
	va->var->i = 0;
	if (!ft_isspace(va->var->c))
	{
		va->var->buffer[va->var->i++] = va->var->c;
		va->var->buffer[va->var->i] = '\0';
	}
	return (1);
}

int	fill3_fourth(t_sp_var *va)
{
	if (va->var->i > 0 && va->var->buffer[0] == '<' && va->var->c != '<')
		return (fill3_fourth_fil(va));
	else if (va->var->i > 0 && va->var->buffer[0] == '>' && va->var->c != '>')
	{
		va->var->buffer[va->var->i] = '\0';
		if (!add_token_with_type(va))
		{
			va->var->i = 0;
			return (0);
		}
		va->var->i = 0;
		if (!ft_isspace(va->var->c))
		{
			va->var->buffer[va->var->i++] = va->var->c;
			va->var->buffer[va->var->i] = '\0';
		}
		return (1);
	}
	else if (va->var->i > 0 && va->var->buffer[0] == '|' && va->var->c != '|')
		return (fill_fill3_fourth(va));
	else
		return (2);
}

static int	fourth_condition_fil(int a, int b, int c, t_sp_var *va)
{
	if (a != 2)
	{
		if (a == 0)
			return (0);
	}
	else if (b != 2)
	{
		if (b == 0)
			return (0);
	}
	else if (c != 2)
	{
		if (c == 0)
			return (0);
	}
	else if (va->var->i == 0 && !ft_isspace(va->var->c))
		va->var->buffer[va->var->i++] = va->var->c;
	return (1);
}

int	fourth_condition(t_sp_var *va)
{
	int	a;
	int	b;
	int	c;

	a = -1;
	b = -1;
	c = -1;
	va->var->wait_more_args = false;
	a = fill1_fourth(va);
	if (a == 2)
		b = fill2_fourth(va);
	if (a == 2 && b == 2)
		c = fill3_fourth(va);
	return (fourth_condition_fil(a, b, c, va));
}
