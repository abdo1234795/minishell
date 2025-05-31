/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:31:08 by abel-had          #+#    #+#             */
/*   Updated: 2025/05/27 11:26:20 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_cmd	*create_new_command(t_sp_var *va)
{
	int		i;
	t_cmd	*cmd;

	cmd = mmallocc(sizeof(t_cmd), &va->allocs, P_GARBAGE);
	i = -1;
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(t_cmd));
	cmd->name = NULL;
	cmd->arg_count = 0;
	cmd->arg_capacity = 10;
	cmd->args = mmallocc(sizeof(char *) * (cmd->arg_capacity + 1),
			&va->allocs, P_GARBAGE);
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	while (++i <= cmd->arg_capacity)
		cmd->args[i] = NULL;
	cmd->am = false;
	cmd->in = NULL;
	cmd->out = NULL;
	cmd->next = NULL;
	return (cmd);
}

t_red	*create_redirection(char *fil, int type, t_sp_var *va)
{
	t_red	*redir;

	redir = mmallocc(sizeof(t_red), &va->allocs, P_GARBAGE);
	if (!redir)
		return (NULL);
	redir->file = ft_strdup(fil, &va->allocs, P_GARBAGE);
	redir->type = type;
	redir->fd = -11;
	redir->next = NULL;
	return (redir);
}

void	init_vpt(t_token *tokens, t_pt *vpt)
{
	ft_memset(vpt, 0, sizeof(t_pt));
	vpt->commands = NULL;
	vpt->current_cmd = NULL;
	vpt->current = tokens;
	vpt->prev = NULL;
	vpt->s = -1;
	vpt->tmp_err = 0;
	vpt->err = 0;
	vpt->syn_err = false;
	vpt->err_file = NULL;
	vpt->result = -1;
	vpt->a = -1;
}

int	ft_detect_syn_err(t_token *tokens, t_sp_var *va)
{
	while (va->vpt->current)
	{
		if (va->vpt->current->syn_err)
			va->vpt->syn_err = true;
		va->vpt->prev = va->vpt->current;
		va->vpt->current = va->vpt->current->next;
	}
	va->vpt->current = tokens;
	va->vpt->commands = create_new_command(va);
	if (!va->vpt->commands)
		return (0);
	va->vpt->current_cmd = va->vpt->commands;
	return (1);
}
