/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 09:21:39 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/31 11:56:13 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

int	reset_g(t_sp_var *v)
{
	signals();
	if (g_signal_pid == -1)
	{
		v->status = 1;
		g_signal_pid = 0;
	}
	if (!isatty(0) || !isatty(1))
		return (1);
	return (0);
}

void	check_line(t_sp_var *v)
{
	v->line = readline("minishell> ");
	if (!v->line)
	{
		v->a = 1;
		v->line = ft_strdup("exit", &v->allocs, P_GARBAGE);
	}
	if (v->line[0] != '\0')
		add_history(v->line);
}

void	status_manage(t_sp_var *v)
{
	if (v->tmp == -3)
		v->status = 0;
	v->tmp = 0;
	v->cmds = parse(v);
	if (v->a != 1)
		free(v->line);
	if (v->status == -3)
		v->tmp = -3;
}

void	process_commands(t_tools *tools, t_sp_var *v)
{
	if (v->cmds)
	{
		tools->cmd = v->cmds;
		ft_execute(tools);
		tools->cmd = v->cmds;
		clean_files(tools);
	}
	else if (v->status == -3)
		g_signal_pid = 0;
	else if (v->status != 999)
		v->status = 258;
	else if (v->status == 999)
		v->status = 1;
}

void	main_loop(t_tools *tools, t_sp_var *v, struct termios *terminal)
{
	while (1)
	{
		if (reset_g(v))
			return ;
		check_line(v);
		if (*v->line != '\0')
		{
			if (g_signal_pid == 3 || g_signal_pid == 2)
				g_signal_pid = 0;
			status_manage(v);
			process_commands(tools, v);
			clean_garbage(tools->aloc);
		}
		else if (v->line && v->line[0] == '\0')
			free(v->line);
		v->a = 0;
		tcsetattr(0, TCSANOW, terminal);
	}
}
