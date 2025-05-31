/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_errors0.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:21:50 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/28 12:23:01 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	cmd_file_error(char *path, char *msg)
{
	print_error("minishell: ");
	print_error(path);
	print_error(": ");
	print_error(msg);
	print_error("\n");
}

void	execve_error(t_tools *tools)
{
	print_error("minishell: ");
	print_error(tools->cmd->name);
	if (errno == ENOENT)
	{
		print_error(": command not found\n");
		*(tools->r_stat) = 127;
	}
	else if (errno == EACCES)
	{
		print_error(": Permission denied\n");
		*(tools->r_stat) = 126;
	}
	else
	{
		print_error(": ");
		print_error(strerror(errno));
		print_error("\n");
		*(tools->r_stat) = 1;
	}
}

int	file_error_handler(char *path, int *status)
{
	struct stat	info;

	if (stat(path, &info) == 0)
	{
		if (S_ISDIR(info.st_mode) == true)
			cmd_file_error(path, "is a Directorie");
		else if (access(path, X_OK) == 0)
			return (1);
		else
			cmd_file_error(path, "permission denied");
		*status = 126;
	}
	else
	{
		cmd_file_error(path, "No such file or directory");
		*status = 127;
	}
	return (0);
}

void	no_cmd_error(t_tools *tools)
{
	print_error("minishell: ");
	print_error(tools->cmd->name);
	print_error(": command not found\n");
	*(tools->r_stat) = 127;
}
