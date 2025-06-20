/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:39:34 by abel-had          #+#    #+#             */
/*   Updated: 2025/06/02 15:03:45 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	expand_args_array(t_cmd *cmd, t_sp_var *va)
{
	int		i;
	char	**new_args;

	cmd->arg_capacity *= 2;
	new_args = mmallocc(sizeof(char *) * (cmd->arg_capacity + 1),
			&va->allocs, P_GARBAGE);
	if (!new_args)
		return ;
	i = -1;
	while (++i < cmd->arg_count)
		new_args[i] = cmd->args[i];
	i = cmd->arg_count - 1;
	while (++i <= cmd->arg_capacity)
		new_args[i] = NULL;
	cmd->args = new_args;
}

void	add_argument(t_cmd *cmd, char *arg, t_sp_var *va)
{
	if (!cmd || !arg)
		return ;
	if (cmd->arg_count == 0)
		cmd->name = ft_strdup(arg, &va->allocs, P_GARBAGE);
	if (!cmd->name)
		return ;
	if (cmd->arg_count >= cmd->arg_capacity)
		expand_args_array(cmd, va);
	cmd->args[cmd->arg_count] = ft_strdup(arg, &va->allocs, P_GARBAGE);
	cmd->arg_count++;
	cmd->args[cmd->arg_count] = NULL;
}
