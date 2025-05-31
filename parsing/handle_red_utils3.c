/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_red_utils3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:28:49 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/27 15:59:21 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	add_red_file(t_cmd *cmd, t_sp_var *va)
{
	if (!cmd->out)
		cmd->out = va->hrv->redir;
	else
	{
		va->hrv->current = cmd->out;
		while (va->hrv->current->next)
			va->hrv->current = va->hrv->current->next;
		va->hrv->current->next = va->hrv->redir;
	}
}

void	while_print_err(t_sp_var *va, int *x)
{
	if (va->vpt->tmp_err == 2 && *x == 0)
	{
		print_error("minishell: ");
		print_error(va->vpt->current->next->value);
		write(2, ": ", 2);
		print_error(strerror(va->vpt->tmp_err));
		write(2, "\n", 1);
		va->status = 1;
		va->vpt->s = 0;
		*x = 1;
	}
}

void	while_part_if_2_fi(t_sp_var *va, int *x)
{
	if (va->vpt->tmp_err == 2)
	{
		if (va->vpt->current_cmd)
			va->vpt->current_cmd->name = NULL;
		while_print_err(va, x);
	}
	if (va->vpt->tmp_err == -3)
		va->status = -3;
	if (va->vpt->err == 0)
	{
		va->vpt->err = va->vpt->tmp_err;
		va->vpt->err_file = va->vpt->current->next->value;
	}
}
