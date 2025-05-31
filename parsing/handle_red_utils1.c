/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_red_utils1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:22:46 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/31 10:59:45 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	init_heredoc_buffer(t_sp_var *va)
{
	va->hrv->capacity = 10;
	va->hrv->bib = mmallocc(sizeof(char *) * va->hrv->capacity,
			&va->allocs, P_GARBAGE);
	if (!va->hrv->bib)
		return (errno);
	return (0);
}

void	expand_heredoc_line(t_sp_var *va, char **tmp)
{
	if (va->hrv->i == 1)
	{
		va->var->state = UNQUOTED;
		va->hrv->tmp = expand_env_vars(*tmp, va);
		*tmp = va->hrv->tmp;
	}
}

int	resize_heredoc_buffer(t_sp_var *va)
{
	if (va->hrv->in >= va->hrv->capacity - 1)
	{
		va->hrv->capacity *= 2;
		va->hrv->new_bib = mmallocc(sizeof(char *) * va->hrv->capacity,
				&va->allocs, P_GARBAGE);
		if (!va->hrv->new_bib)
			return (errno);
		va->hrv->j = -1;
		while (++va->hrv->j < va->hrv->in)
			va->hrv->new_bib[va->hrv->j] = va->hrv->bib[va->hrv->j];
		va->hrv->bib = va->hrv->new_bib;
	}
	return (0);
}

int	heredoc_to_temp_file(t_sp_var *va)
{
	int	a;

	g_signal_pid = 0;
	va->hrv->bib[va->hrv->in] = NULL;
	va->hrv->fd = open("t_m_p_f_i_l_e", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (va->hrv->fd < 0)
		return (errno);
	va->hrv->j = -1;
	while (++va->hrv->j < va->hrv->in)
	{
		write(va->hrv->fd, va->hrv->bib[va->hrv->j],
			ft_strlen(va->hrv->bib[va->hrv->j]));
		write(va->hrv->fd, "\n", 1);
	}
	close(va->hrv->fd);
	va->hrv->redir->fd = open("t_m_p_f_i_l_e", O_RDONLY);
	a = unlink("t_m_p_f_i_l_e");
	if (a)
		perror("minishell: unlink: ");
	return (0);
}
