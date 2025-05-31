/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_builts2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 10:14:18 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/28 16:22:18 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	exit_numeric_error(char *arg, t_tools *tools)
{
	errno = 0;
	print_error("minishell: exit: ");
	print_error(arg);
	print_error(": numeric argument required\n");
	clean_up(tools);
	exit(255);
}

void	failed_exit_args_num(int *status)
{
	print_error("minishell: exit: too many arguments\n");
	*status = 1;
}

int	handle_cd_failed_cwd(t_env **env, t_tools *tools, \
		t_malloc **a, char *to_path)
{
	char	*oldpwd;

	print_error("cd: error retrieving current directory: ");
	perror("getcwd: cannot access parent directories");
	oldpwd = get_key_value("PWD", *env);
	if (oldpwd && ft_strlen(oldpwd))
	{
		update_var(tools, ft_strdup(oldpwd, a, P_ENV), "OLDPWD");
		update_var(tools, ft_strdup(ft_strjoin(ft_strjoin(oldpwd, "/", a) \
		, to_path, a), a, P_ENV), "p.a.t.h");
		update_var(tools, ft_strdup(ft_strjoin(ft_strjoin(oldpwd, "/", a) \
		, to_path, a), a, P_ENV), "PWD");
	}
	else if (oldpwd)
	{
		update_var(tools, ft_strdup(oldpwd, a, P_ENV), "OLDPWD");
		update_var(tools, ft_strdup(ft_strjoin(oldpwd, to_path, a) \
		, a, P_ENV), "p.a.t.h");
		update_var(tools, ft_strdup(ft_strjoin(oldpwd, to_path, a) \
		, a, P_ENV), "PWD");
	}
	return (0);
}

int	handle_home(t_tools *tools)
{
	char		*home;

	home = get_key_value("HOME", *(tools->env));
	if (!home)
		return (print_error("minishell: cd: HOME not set\n"), 1);
	if (chdir(home) == -1)
	{
		cd_error(home);
		return (1);
	}
	else
	{
		ft_pwd(tools->env, tools->aloc, home, tools);
	}
	return (0);
}
