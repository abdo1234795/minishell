/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:24:27 by aelbour           #+#    #+#             */
/*   Updated: 2025/06/02 17:48:48 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_garbage(t_malloc **aloc)
{
	t_malloc	*cursor;
	t_malloc	*next;

	cursor = *aloc;
	while (cursor)
	{
		next = cursor->next;
		if (cursor->p_type == P_GARBAGE)
			free_ptr(aloc, cursor->ptr);
		cursor = next;
	}
}

void	clean_up(t_tools *tools)
{
	t_malloc	*nxt;

	clean_files(tools);
	if (!(tools->aloc) || !(*tools->aloc))
		return ;
	while ((*tools->aloc))
	{
		free((*tools->aloc)->ptr);
		nxt = (*tools->aloc)->next;
		free((*tools->aloc));
		(*tools->aloc) = nxt;
	}
	(*tools->aloc) = NULL;
	free(tools->v);
	rl_clear_history();
}

void	clean_files(t_tools *tools)
{
	t_red	*cursor;
	t_cmd	*cmd_cur;

	cmd_cur = tools->cmd_head;
	while (cmd_cur)
	{
		cursor = cmd_cur->in;
		while (cursor)
		{
			if (cursor->fd > 0)
				close(cursor->fd);
			cursor = cursor->next;
		}
		cursor = cmd_cur->out;
		while (cursor)
		{
			if (cursor->fd > 0)
				close(cursor->fd);
			cursor = cursor->next;
		}
		cmd_cur = cmd_cur->next;
	}
}
