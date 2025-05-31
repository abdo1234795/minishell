/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_errors1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 09:43:55 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/28 16:50:41 by aelbour          ###   ########.fr       */
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
