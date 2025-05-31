/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builts3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 10:18:40 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/28 16:21:52 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	handle_old_path(t_tools *tools)
{
	char		*oldpwd;

	oldpwd = get_key_value("OLDPWD", *(tools->env));
	if (!oldpwd)
		return (print_error("minishell: cd: OLDPWD not set\n"), 1);
	if (chdir(oldpwd) == -1)
	{
		cd_error(oldpwd);
		return (1);
	}
	else
	{
		ft_pwd(tools->env, tools->aloc, oldpwd, tools);
		ft_pwd(tools->env, NULL, NULL, tools);
	}
	return (0);
}

int	treat_variable(t_tools *tools, char *key, char *value, int i)
{
	int		check;
	int		status;

	status = 0;
	check = var_action(key, value, *(tools->env));
	if (tools->cmd->args[i][0] == '=')
		check = 3;
	if (check == 1)
		push_to_env(tools, key, value);
	else if (check == 2)
		update_var(tools, ft_strdup(value, tools->aloc, P_ENV), key);
	else if (check == 4)
		append_value(tools, key, value);
	else if (check == 3)
		status = ft_export_var_error(tools->cmd->args[i]);
	return (status);
}
