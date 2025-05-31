/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_3utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 10:35:39 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/27 10:43:55 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*remove_character(t_sp_var *va, char c)
{
	char	*new_str;
	int		i;
	int		j;

	i = -1;
	j = 0;
	if (!va->var->buffer)
		return (NULL);
	new_str = mmallocc(ft_strlen(va->var->buffer) + 1, &va->allocs, P_GARBAGE);
	if (!new_str)
		return (NULL);
	while (va->var->buffer[++i])
	{
		if (va->var->buffer[i] != c)
			new_str[j++] = va->var->buffer[i];
	}
	new_str[j] = '\0';
	return (new_str);
}

int	check_operator_validity(char *buffer, t_quote_state *state)
{
	if (*state == UNQUOTED && buffer[0] == '>' && ft_strlen(buffer) <= 2)
		return (1);
	if (*state == UNQUOTED && buffer[0] == '<' && ft_strlen(buffer) <= 2)
		return (1);
	if (*state == UNQUOTED && buffer[0] == '|' && ft_strlen(buffer) == 1)
		return (1);
	return (0);
}

void	add_error_token(t_token **token, t_sp_var *va)
{
	t_token	*current;
	t_token	*new_token;

	new_token = mmallocc(sizeof(t_token), &va->allocs, P_GARBAGE);
	if (!new_token)
		return ;
	new_token->value = ft_strdup(va->var->buffer, &va->allocs, P_GARBAGE);
	new_token->type = text;
	new_token->syn_err = true;
	new_token->heredoc = false;
	new_token->wait_more_args = false;
	new_token->newline = true;
	new_token->next = NULL;
	if (*token == NULL)
		*token = new_token;
	else
	{
		current = *token;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
}

void	detect_file_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*previous;

	current = *tokens;
	previous = NULL;
	while (current != NULL)
	{
		if (current->type == text && previous != NULL && previous->type == red)
			current->type = file;
		previous = current;
		current = current->next;
	}
}

int	is_token_separator(char c)
{
	return (ft_isspace(c) || c == '>' || c == '<' || c == '|');
}
