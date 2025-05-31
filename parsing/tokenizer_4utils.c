/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_4utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 10:42:34 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/27 10:56:26 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	handle_initial_case(t_token *previous, t_token *current)
{
	if (previous->type == pip || (previous->type == red
			&& (previous->next == NULL || previous->next->type != file)))
	{
		previous->syn_err = true;
		previous->newline = (current != NULL);
		previous->next = NULL;
		return (0);
	}
	return (1);
}

static int	check_invalid_sequence(t_token *prev, t_token *curr)
{
	if (curr->type == pip && (prev->type == pip || prev->type == red))
		return (1);
	if (curr->type == red && prev->type == red)
		return (1);
	if (prev->type == red && curr->type != file)
		return (1);
	if (prev->type == red && !ft_strcmp(prev->value, "<<")
		&& curr->type != file && curr->type != text)
		return (1);
	return (0);
}

static int	handle_final_case(t_token *previous)
{
	if (previous->type == pip || previous->type == red)
	{
		previous->syn_err = true;
		previous->next = NULL;
		return (0);
	}
	return (1);
}

int	validate_syntax(t_token **tokens)
{
	t_token	*current;
	t_token	*previous;

	if (!tokens || !*tokens)
		return (1);
	current = *tokens;
	previous = current;
	current = current->next;
	if (!handle_initial_case(previous, current))
		return (0);
	while (current)
	{
		if (check_invalid_sequence(previous, current))
		{
			previous->syn_err = true;
			previous->newline = true;
			previous->next = NULL;
			return (0);
		}
		previous = current;
		current = current->next;
	}
	return (handle_final_case(previous));
}
