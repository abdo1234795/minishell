/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:46:47 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/26 18:47:06 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_cmd	*parse(t_sp_var *v)
{
	t_token	*tokens;
	t_cmd	*commands;

	tokens = tokenize_input(v);
	if (!tokens)
		return (NULL);
	commands = parse_tokens(tokens, v);
	if (!commands)
		return (NULL);
	return (commands);
}
