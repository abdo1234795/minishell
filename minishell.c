/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 08:51:06 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/28 11:07:30 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

int	init_v(t_sp_var **v)
{
	*v = malloc(sizeof(t_sp_var));
	if (!*v)
		return (write(2, "minishell: malloc failed\n", 25), 0);
	ft_memset(*v, 0, sizeof(t_sp_var));
	(*v)->st_quote = false;
	(*v)->st_ambiguous = false;
	(*v)->st_ambiguous_red = false;
	(*v)->line = NULL;
	(*v)->cmds = NULL;
	(*v)->env = NULL;
	(*v)->allocs = NULL;
	(*v)->status = 0;
	(*v)->tmp = -1;
	(*v)->a = 0;
	(*v)->vpt = NULL;
	return (1);
}

void	init_pwd_variables(t_tools *tools)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
	{
		perror("minshell-init: error retrieving current directory: getcwd");
		update_var(tools, NULL, "p.a.t.h");
	}
	else if (path)
	{
		update_var(tools, ft_strdup(path, tools->aloc, 0), "p.a.t.h");
		update_var(tools, ft_strdup(path, tools->aloc, 0), "PWD");
	}
	free(path);
	path = get_key_value("OLDPWD", *(tools->env));
	update_var(tools, NULL, "OLDPWD");
}

void	init_env_variables(t_tools *tools, t_sp_var *v)
{
	char	*shlvl;
	char	*path;

	init_pwd_variables(tools);
	shlvl = get_key_value("SHLVL", *(tools->env));
	if (shlvl)
		update_var(tools, ft_strdup(ft_itoa(ft_atoi(shlvl) + 1, v) \
		, tools->aloc, P_ENV), "SHLVL");
	else
		push_to_env(tools, "SHLVL", "1");
	path = get_key_value("PATH", *tools->env);
	if (!path)
		push_to_env(tools, "PATH", SECURE_PATH);
}

void	init_execution_tools(t_tools *tools, t_sp_var *v, char **envp)
{
	tools->cmd = NULL;
	tools->aloc = &(v->allocs);
	tools->r_stat = &(v->status);
	tools->env = &(v->env);
	tools->envp = envp;
	tools->v = v;
	push_envp(tools);
	init_env_variables(tools, v);
}

int	main(int argc, char **argv, char **envp)
{
	t_sp_var		*v;
	t_tools			tools;
	struct termios	terminal;

	(void)argc;
	(void)argv;
	if (!init_v(&v))
		return (0);
	init_execution_tools(&tools, v, envp);
	tcgetattr(0, &terminal);
	main_loop(&tools, v, &terminal);
	rl_clear_history();
	return (0);
}
