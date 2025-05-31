/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:40:36 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/29 16:39:39 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	while_part_if_1(t_sp_var *va, int *x)
{
	if (va->vpt->err != 0 && va->vpt->tmp_err != -2
		&& g_signal_pid != -2 && va->vpt->err != -3
		&& !va->vpt->current_cmd->am && *x == 0)
	{
		print_error("minishell: ");
		print_error(va->vpt->err_file);
		write(2, ": ", 2);
		print_error(strerror(va->vpt->err));
		write(2, "\n", 1);
		va->status = 1;
		*x = 1;
	}
	va->vpt->current_cmd->next = create_new_command(va);
	if (!va->vpt->current_cmd->next)
		return (0);
	va->vpt->s = 1;
	va->vpt->current_cmd = va->vpt->current_cmd->next;
	return (1);
}

int	while_part_if_2_fill(t_sp_var *va)
{
	if (va->vpt->tmp_err == -2)
	{
		if (va->vpt->current_cmd && va->vpt->current_cmd->name)
		{
			va->vpt->current_cmd->name = NULL;
			return (2);
		}
	}
	if (va->vpt->tmp_err != 0)
	{
		va->vpt->s = 0;
		if (va->vpt->current_cmd && va->vpt->current_cmd->name)
			va->vpt->current_cmd->name = NULL;
	}
	return (1);
}

int	while_part_if_2(t_sp_var *va, int *x)
{
	if (!va->vpt->current->next || va->vpt->current->next->type != file)
	{
		print_error("minishell: syntax error: ");
		print_error("Missing filename after redirection\n");
		return (0);
	}
	else if (va->vpt->current->next && va->vpt->current->next->type == file)
	{
		va->vpt->tmp_err = handle_redirection(va->vpt->current_cmd,
				va->vpt->current, va, va->vpt->s);
		while_part_if_2_fi(va, x);
		if (va->vpt->tmp_err == -2)
			return (10);
		if (while_part_if_2_fill(va) == 2)
			return (2);
		va->vpt->current = va->vpt->current->next;
	}
	return (1);
}

void	while_part_print_err(t_sp_var *va, int *x)
{
	if (va->vpt->err != 0 && va->vpt->tmp_err != -2
		&& g_signal_pid != -2 && va->vpt->err != -3
		&& !va->vpt->current_cmd->am && *x == 0)
	{
		print_error("minishell: ");
		print_error(va->vpt->err_file);
		write(2, ": ", 2);
		print_error(strerror(va->vpt->err));
		write(2, "\n", 1);
		va->status = 1;
		va->vpt->s = 0;
		*x = 1;
	}
}

int	while_part(t_sp_var *va)
{
	int	x;
	int	a;

	x = 0;
	while (va->vpt->current && !va->vpt->current->syn_err)
	{
		a = while_part_fiill(va, &x);
		if (a != 1)
			return (a);
	}
	while_part_print_err(va, &x);
	if (va->vpt->syn_err)
		return (0);
	return (1);
}
