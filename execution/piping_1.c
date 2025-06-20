/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:45:45 by aelbour           #+#    #+#             */
/*   Updated: 2025/06/02 13:07:50 by aelbour          ###   ########.fr       */
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
