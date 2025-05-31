/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_red_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:25:32 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/31 12:05:51 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	read_heredoc_lines(t_sp_var *va)
{
	int		result;
	int		fd;
	char	*tmp;
	int		line_result;

	if (setup_heredoc_reading(&fd) != 0)
		return (-1);
	while (1)
	{
		line_result = process_heredoc_line(va, &tmp, fd);
		if (line_result != 1)
			return (line_result);
		expand_heredoc_line(va, &tmp);
		result = resize_heredoc_buffer(va);
		if (result != 0)
			return (return_tesult(result, fd));
		va->hrv->bib[va->hrv->in++] = tmp;
		tmp = NULL;
	}
	if (result < 0)
		return (result);
	return (0);
}

int	process_heredoc(t_sp_var *va)
{
	int	result;

	result = init_heredoc_buffer(va);
	if (result != 0)
		return (result);
	result = read_heredoc_lines(va);
	if (result != 0)
		return (result);
	return (heredoc_to_temp_file(va));
}

int	handle_red_if1(t_cmd *cmd, t_sp_var *va)
{
	int	result;

	result = 0;
	(void) cmd;
	if (va->hrv->redir_type == REDIR_HEREDOC)
	{
		if (g_signal_pid != 3)
			result = process_heredoc(va);
		else
			return (10);
	}
	else if (va->hrv->redir_type == REDIR_IN)
	{
		va->hrv->redir->fd = open(va->hrv->redir->file, O_RDONLY);
	}
	if (va->hrv->redir->fd != -11 && va->hrv->redir->fd != -1
		&& va->vpt->current_cmd->am)
		va->hrv->redir->fd = -11;
	return (result);
}

int	handle_red_if2(t_cmd *cmd, t_sp_var *va)
{
	if (va->hrv->redir->fd == -1 || va->hrv->redir->fd == -11)
		return (errno);
	if (!cmd->in)
		cmd->in = va->hrv->redir;
	else
	{
		va->hrv->current = cmd->in;
		while (va->hrv->current->next)
			va->hrv->current = va->hrv->current->next;
		va->hrv->current->next = va->hrv->redir;
	}
	return (0);
}

int	open_outfile(t_sp_var *va)
{
	if (va->hrv->redir_type == REDIR_APPEND && !va->vpt->current_cmd->am)
		va->hrv->redir->fd = open(va->hrv->redir->file, O_WRONLY
				| O_CREAT | O_APPEND, 0644);
	else if (!va->vpt->current_cmd->am)
		va->hrv->redir->fd = open(va->hrv->redir->file, O_WRONLY
				| O_CREAT | O_TRUNC, 0644);
	if (va->hrv->redir->fd != -11 && va->hrv->redir->fd != -1
		&& va->vpt->current_cmd->am)
		va->hrv->redir->fd = -11;
	if (va->hrv->redir->fd == -1)
		return (errno);
	return (0);
}
