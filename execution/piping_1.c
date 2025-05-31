/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:45:45 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/29 10:09:36 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	execute_piped_cmd(t_tools *tools)
{
	int		i;
	int		n;

	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	if (!tools->cmd->name)
		exit(*(tools->r_stat));
	i = is_builtins(tools->cmd->name);
	if (i)
		(execute_builtin(i, tools), n = *tools->r_stat, \
			clean_up(tools), exit(n));
	else if (ft_strchr(tools->cmd->name, '/'))
	{
		tools->envp = vars_to_envp(tools);
		if (execve(tools->cmd->name, tools->cmd->args, tools->envp) == -1)
			return (execve_error(tools), n = *tools->r_stat, clean_up(tools), \
					exit(n));
	}
	else
		exec_no_path_cmd_pipe(tools);
}

void	exec_no_path_cmd_pipe(t_tools *tools)
{
	char	*path;
	int		n;

	path = get_executable_path(tools->cmd->name, tools->aloc, *(tools->env));
	if (path)
	{
		tools->envp = vars_to_envp(tools);
		tools->cmd->name = path;
		if (execve(tools->cmd->name, tools->cmd->args, tools->envp) == -1)
			return (execve_error(tools), n = *tools->r_stat, clean_up(tools), \
					exit(n));
	}
}

void	close_fds(int pipe_count, int **arr, t_tools *tools)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (i < pipe_count)
	{
		if (close(arr[i][0]) == -1 || close(arr[i][1]) == -1)
			critical_error("close", tools, 0, tools->r_stat);
		i++;
	}
}

void	close_pipes(int **arr, int up_to)
{
	int	i;

	i = 0;
	while (i < up_to)
	{
		close(arr[i][0]);
		close(arr[i][1]);
		i++;
	}
}

int	**get_pipe_buff(t_tools *tools, int count)
{
	int	**arr;
	int	i;

	arr = mmallocc(sizeof(int *) * (count - 1), tools->aloc, P_GARBAGE);
	i = 0;
	while (i < count - 1)
	{
		arr[i] = mmallocc(2 * sizeof(int), tools->aloc, P_GARBAGE);
		arr[i][0] = 0;
		arr[i][1] = 1;
		i++;
	}
	return (arr);
}
