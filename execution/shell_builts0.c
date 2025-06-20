/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builts0.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:46:22 by aelbour           #+#    #+#             */
/*   Updated: 2025/06/19 07:53:53 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	ft_cd(t_tools *tools)
{
	if (tools->cmd->args[1] == NULL || !ft_strcmp("~", tools->cmd->args[1]))
		return (handle_home(tools));
	else if (!ft_strcmp(tools->cmd->args[1], "-"))
		return (handle_old_path(tools));
	else
	{
		if (chdir(tools->cmd->args[1]) == -1)
		{
			cd_error(tools->cmd->args[1]);
			return (1);
		}
		else
		{
			ft_pwd(tools->env, tools->aloc, tools->cmd->args[1], tools);
		}
	}
	return (0);
}

int	ft_export(t_tools *tools)
{
	int		i;
	int		status;
	char	*value;
	char	*key;

	i = 0;
	status = 0;
	while (tools->cmd->args[++i])
	{
		key = ft_split(tools->cmd->args[i], '=', tools->aloc)[0];
		value = ft_strchr(tools->cmd->args[i], '=');
		if (value)
			value++;
		status = treat_variable(tools, key, value, i);
	}
	if (i == 1)
		export_display(tools->env, tools->aloc);
	return (status);
}

int	ft_env(t_malloc **aloc, t_env **env, t_cmd *cmd)
{
	t_env	*ptr;

	(void)aloc;
	ptr = *env;
	if (cmd->args[1])
	{
		print_error("env:");
		print_error(cmd->args[1]);
		print_error("too many arguments\n");
		return (1);
	}
	while (ptr)
	{
		if (ptr->value && is_key_valid(ptr->key))
			printf("%s=%s\n", ptr->key, ptr->value);
		ptr = ptr->next;
	}
	return (0);
}

int	ft_unset(t_cmd *cmd, t_malloc **aloc, t_env **env)
{
	int		i;
	int		status;

	i = 0;
	status = 0;
	while (cmd->args[++i])
	{
		if (is_var_exist(cmd->args[i], *env))
			remove_variable(cmd->args[i], env, aloc);
		if (!is_key_valid(cmd->args[i]))
		{
			print_error("minishell: unset: `");
			print_error(cmd->args[i]);
			print_error("': not a valid identifier\n");
			status = 1;
		}
	}
	return (status);
}

void	ft_exit(t_tools *tools, int *status, int n)
{
	char		*s;

	if (tools->will_exit)
		write(1, "exit\n", 5);
	if (tools->cmd->args[1])
	{
		s = ft_isnum(tools->cmd->args[1], tools->aloc);
		if (s && !tools->cmd->args[2])
			handle_numric_arg_exit(tools, s);
		else if (s)
			failed_exit_args_num(status);
		else
			exit_numeric_error(tools->cmd->args[1], tools);
	}
	else
		return (clean_up(tools), exit(n));
}
