/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbour <aelbour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:16:42 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/30 11:19:49 by aelbour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	safe_pipe_error(char *error, int **arr, t_tools *tools, int num)
{
	critical_error(error, tools, 0, tools->r_stat);
	kill(0, SIGINT);
	while (wait(NULL) != -1)
		;
	*(tools->r_stat) = 1;
	close_pipes(arr, num);
}
