/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_fill2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:00:35 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/27 11:15:01 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	fill1_fourth_fil(t_sp_var *va)
{
	va->var->buffer[va->var->i] = '\0';
	if (!add_token_with_type(va))
	{
		va->var->i = 0;
		return (0);
	}
	va->var->i = 0;
	va->var->buffer[va->var->i] = '\0';
	if (!ft_isspace(va->var->c))
	{
		va->var->buffer[va->var->i++] = va->var->c;
		va->var->buffer[va->var->i] = '\0';
	}
	return (1);
}

int	fill1_fourth(t_sp_var *va)
{
	if (va->var->i > 0 && va->var->buffer[0] != '<'
		&& va->var->buffer[0] != '>' && va->var->buffer[0] != '|')
		return (fill1_fourth_fil (va));
	else if (va->var->i > 0 && va->var->buffer[0] == '<' && va->var->c == '<')
	{
		va->var->buffer[va->var->i++] = va->var->c;
		if (!va->line[va->var->j + 1])
		{
			va->var->buffer[va->var->i] = '\0';
			if (!add_token_with_type(va))
			{
				va->var->i = 0;
				return (0);
			}
			va->var->i = 0;
			va->var->buffer[0] = '\0';
			return (1);
		}
	}
	return (2);
}

static int	fill2_fourth_fil(t_sp_var *va)
{
	va->var->buffer[va->var->i++] = va->var->c;
	if (!va->line[va->var->j + 1])
	{
		va->var->buffer[va->var->i] = '\0';
		if (!add_token_with_type(va))
		{
			va->var->i = 0;
			return (0);
		}
		va->var->i = 0;
		va->var->buffer[0] = '\0';
		return (1);
	}
	return (1);
}

int	fill2_fourth(t_sp_var *va)
{
	if (va->var->i > 0 && va->var->buffer[0] == '>' && va->var->c == '>')
		return (fill2_fourth_fil(va));
	else if (va->var->i > 0 && va->var->buffer[0] == '|' && va->var->c == '|')
	{
		va->var->buffer[va->var->i++] = va->var->c;
		if (!va->line[va->var->j + 1])
		{
			va->var->buffer[va->var->i] = '\0';
			if (!add_token_with_type(va))
			{
				va->var->i = 0;
				return (0);
			}
			va->var->i = 0;
			va->var->buffer[0] = '\0';
			return (1);
		}
	}
	return (2);
}

int	fill_fill3_fourth(t_sp_var *va)
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
