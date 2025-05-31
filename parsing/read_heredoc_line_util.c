/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc_line_util.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:12:40 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/31 12:06:17 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	setup_heredoc_reading(int *fd)
{
	if (g_signal_pid != 2)
		g_signal_pid = 0;
	*fd = dup(0);
	if (*fd == -1)
		return (-1);
	signal(SIGINT, heredoc_signal);
	return (0);
}

int	sig_return(int fd)
{
	if (g_signal_pid == 3)
	{
		dup2(fd, 0);
		close(fd);
		return (-2);
	}
	g_signal_pid = 2;
	return (-3);
}

int	process_heredoc_line(t_sp_var *va, char **tmp, int fd)
{
	if (g_signal_pid == 3)
	{
		dup2(fd, 0);
		close(fd);
		return (-2);
	}
	va->hrv->line = readline("> ");
	*tmp = ft_strdup(va->hrv->line, &va->allocs, P_GARBAGE);
	free(va->hrv->line);
	va->hrv->line = NULL;
	if (!*tmp)
		return (sig_return(fd));
	if (ft_strcmp(*tmp, va->hrv->file_token->value) == 0)
	{
		dup2(fd, 0);
		close(fd);
		va->status = 0;
		return (0);
	}
	return (1);
}

int	return_tesult(int result, int fd)
{
	dup2(fd, 0);
	close(fd);
	return (result);
}

void	heredoc_signal(int a)
{
	(void)a;
	close(0);
	if (g_signal_pid != 2)
		write(1, "\n", 1);
	g_signal_pid = 3;
}
