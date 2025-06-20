/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:31:09 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/27 11:25:45 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	fds_backup(int in_backup, int out_backup, int *r_stat)
{
	if (dup2(in_backup, 0) == -1 || dup2(out_backup, 1) == -1)
		critical_error("dup2", NULL, 0, r_stat);
	if (close(in_backup) == -1 || close(out_backup) == -1)
		critical_error("close", NULL, 0, r_stat);
}

void	output_redirection(t_tools *tools)
{
	t_red	*cur;

	cur = tools->cmd->out;
	while (cur->next)
	{
		cur = cur->next;
	}
	if (cur->fd != -11 && cur->fd > 0)
		if (dup2(cur->fd, STDOUT_FILENO) == -1)
			critical_error("dup2", NULL, 0, tools->r_stat);
}

void	input_redirection(t_tools *tools)
{
	t_red	*cur;

	cur = tools->cmd->in;
	while (cur->next)
	{
		cur = cur->next;
	}
	if (cur->fd != -11 && cur->fd > 0)
		if (dup2(cur->fd, STDIN_FILENO) == -1)
			critical_error("dup2", NULL, 0, tools->r_stat);
}

void	redirect_command(t_tools *tools)
{
	if (tools->cmd->out)
	{
		output_redirection(tools);
	}
	if (tools->cmd->in)
	{
		input_redirection(tools);
	}
}
