/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:11:23 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/30 15:13:20 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	check_cmd_valdity(char *str, t_tools *tools)
{
	char	*path;

	if (str && is_builtins(str))
		return (1);
	else if (str && ft_strchr(str, '/'))
	{
		if (file_error_handler(str, tools->r_stat))
			return (1);
	}
	else if (str)
	{
		path = get_executable_path(str, tools->aloc, *(tools->env));
		if (path)
			return (1);
		else
		{
			print_error("minishell: ");
			print_error(tools->cmd->name);
			print_error(": command not found\n");
			*(tools->r_stat) = 127;
		}
	}
	tools->cmd->name = NULL;
	return (0);
}

int	count_cmd_list(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		i++;
		cmd = cmd->next;
	}
	return (i);
}

void	piped_child(t_tools *tools, int cmd_count, int **arr, int num)
{
	int	n;

	(void)cmd_count;
	if (tools->cmd->next)
		if (dup2(arr[num][1], STDOUT_FILENO) == -1)
			return (critical_error("dup2", tools, 1, NULL));
	if (num)
		if (dup2(arr[num - 1][0], STDIN_FILENO) == -1)
			return (critical_error("dup2", tools, 1, NULL));
	if (tools->cmd->next)
		close(arr[num][1]);
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
			return (safe_pipe_error("pipe error", arr, tools, num), -1);
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
