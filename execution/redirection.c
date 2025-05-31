/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:31:09 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/30 15:14:14 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

bool	same_file(const char *path1, const char *path2)
{
	struct stat	s1;
	struct stat	s2;

	if (stat(path1, &s1) < 0 || stat(path2, &s2) < 0)
		return (false);
	return (s1.st_ino == s2.st_ino && s1.st_dev == s2.st_dev);
}

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
	if (cur->fd != -11)
	{
		if (dup2(cur->fd, STDOUT_FILENO) == -1)
		{
			critical_error("dup2", NULL, 0, tools->r_stat);
		}
	}
}

void	input_redirection(t_tools *tools)
{
	t_red	*cur;

	cur = tools->cmd->in;
	while (cur->next)
	{
		cur = cur->next;
	}
	if (!same_file(cur->file, "/dev/stdin"))
	{
		if (cur->fd != -11)
		{
			if (dup2(cur->fd, STDIN_FILENO) == -1)
				critical_error("dup2", NULL, 0, tools->r_stat);
		}
	}
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
