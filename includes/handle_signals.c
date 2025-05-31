/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:29:52 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/30 15:11:33 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal_pid = 0;

static int	handle_sigint_part1(void)
{
	if (g_signal_pid == 4)
		return (0);
	if (g_signal_pid == 3)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		return (0);
	}
	return (1);
}

static void	handle_sigint(int n)
{
	(void)n;
	if (!handle_sigint_part1())
		return ;
	if (g_signal_pid != 3)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_signal_pid = -1;
	}
}

void	signals(void)
{
	signal(SIGINT, &handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
