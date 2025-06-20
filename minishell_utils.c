/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 09:21:39 by abel-had          #+#    #+#             */
/*   Updated: 2025/06/02 18:18:15 by aelbour          ###   ########.fr       */
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
	if (!v->cmds && v->status == 2)
	{
		print_error("minishell: maximum here-document count exceeded\n");
		exit (2);
	}	
	if (v->a != 1)
		free(v->line);
	if (v->status == -3)
		v->tmp = -3;
}

bool	ft_just_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if(str[i] != ' ' && str[i] != '\t')
			return (false);
		i++;
	}
	return (true);
}

void	process_commands(t_tools *tools, t_sp_var *v, bool just_space)
{
	
	if (v->cmds)
	{
		tools->cmd = v->cmds;
		tools->cmd_head = v->cmds;
		ft_execute(tools);
		clean_files(tools);
	}
	else if (v->status == -3)
	g_signal_pid = 0;
	else if (v->status != 999 && !just_space)
	v->status = 258;
	else if (v->status == 999)
	v->status = 1;
}

void	main_loop(t_tools *tools, t_sp_var *v, struct termios *terminal)
{
	bool just_space;
	
	while (1)
	{
		if (reset_g(v))
		return ;
		check_line(v);
		if (*v->line != '\0')
		{
			just_space = ft_just_space(v->line);
			if (g_signal_pid == 3 || g_signal_pid == 2)
				g_signal_pid = 0;
			status_manage(v);
			if (g_signal_pid == -1)
			{
				v->status = 1;
				g_signal_pid = 0;
			}
			process_commands(tools, v, just_space);
			clean_garbage(tools->aloc);
		}
		else if (v->line && v->line[0] == '\0')
			free(v->line);
		v->a = 0;
		tcsetattr(0, TCSANOW, terminal);
	}
}
