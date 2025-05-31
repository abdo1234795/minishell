/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:24:27 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/30 12:29:38 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	critical_error(char *syscall, t_tools *tools, int quit, int *r_stat)
{
	perror(syscall);
	if (quit)
	{
		if (strcmp(syscall, "malloc"))
			clean_up(tools);
		clean_files(tools);
		exit(EXIT_FAILURE);
	}
	else
		*r_stat = 1;
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

void	clean_up(t_tools *tools)
{
	t_malloc	*nxt;

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
