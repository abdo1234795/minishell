/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:11:23 by aelbour           #+#    #+#             */
/*   Updated: 2025/06/16 16:12:47 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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

void	piped_child(t_tools *tools, int cmd_count, int **arr, int num)
{
	int	n;

	tools->will_exit = 0;
	(void)cmd_count;
	if (tools->cmd->next)
		if (dup2(arr[num][1], STDOUT_FILENO) == -1)
			return (critical_error("dup2", tools, 1, NULL));
	if (num)
		if (dup2(arr[num - 1][0], STDIN_FILENO) == -1)
			return (critical_error("dup2", tools, 1, NULL));
	if (tools->cmd->next)
	{
		close(arr[num][1]);
		close(arr[num][0]);
	}
	if (num)
		close(arr[num - 1][0]);
	redirect_command(tools);
	execute_piped_cmd(tools);
	n = *tools->r_stat;
	clean_up(tools);
	exit(n);
}

int	manage_pipes_redirection(t_tools *tools, int cmd_count, \
	int **arr, pid_t pid)
{
	int	num;

	num = 0;
	cmd_count = count_cmd_list(tools->cmd);
	arr = get_pipe_buff(tools, cmd_count);
	while (arr && tools->cmd)
	{
		check_cmd_valdity(tools->cmd->name, tools);
		if (tools->cmd->next && pipe(arr[num]) == -1)
			return (safe_pipe_error("pipe", arr, tools, num), -1);
		pid = fork();
		if (pid == -1)
			return (safe_pipe_error("fork", arr, tools, num), -1);
		else if (pid == 0)
			piped_child(tools, cmd_count, arr, num);
		if (tools->cmd->next)
			close(arr[num][1]);
		if (num)
			close(arr[num - 1][0]);
		tools->cmd = tools->cmd->next;
		num++;
	}
	return (pid);
}

void	execute_pipeline(t_tools *tools)
{
	int		status;
	int		num;
	int		right_most;

	right_most = manage_pipes_redirection(tools, 0, NULL, -1);
	if (right_most < 0)
		return ;
	num = waitpid(right_most, &status, 0);
	if (num == -1)
		critical_error("waitpid", tools, 0, tools->r_stat);
	else if (num == right_most)
	{
		if (WIFEXITED(status))
			*(tools->r_stat) = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*(tools->r_stat) = 128 + WTERMSIG(status);
	}
	while (waitpid(-1, &status, 0) > 0)
		;
	if (WIFSIGNALED(status))
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
}
