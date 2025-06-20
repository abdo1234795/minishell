/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_errors1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 09:43:55 by aelbour           #+#    #+#             */
/*   Updated: 2025/06/02 17:48:38 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	ft_export_var_error(char *key)
{
	print_error("minishell: export: `");
	print_error(key);
	print_error("': not a valid identifier\n");
	return (1);
}

void	cd_error(char *path)
{
	print_error("cd: ");
	print_error(strerror(errno));
	print_error(": ");
	print_error(path);
	print_error("\n");
}

void	print_error(const char *msg)
{
	ft_putstr_fd((char *)msg, 2);
}

void	critical_error(char *syscall, t_tools *tools, int quit, int *r_stat)
{
	perror(syscall);
	if (quit)
	{
		if (strcmp(syscall, "malloc"))
			clean_up(tools);
		exit(EXIT_FAILURE);
	}
	else
		*r_stat = 1;
}
