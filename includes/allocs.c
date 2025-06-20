/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-had <abel-had@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:33:05 by aelbour           #+#    #+#             */
/*   Updated: 2025/05/28 11:08:08 by abel-had         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*mmallocc(size_t size, t_malloc **head, int p_type)
{
	void		*pointer;
	t_malloc	*new;

	pointer = NULL;
	pointer = malloc(size);
	ft_memset(pointer, 0, size);
	if (!pointer)
		return (critical_error("malloc", NULL, 1, NULL), NULL);
	new = malloc(sizeof(t_malloc));
	if (!new)
	{
		return (free(pointer), critical_error("malloc", NULL, 1, NULL), NULL);
	}
	new->next = (*head);
	new->ptr = pointer;
	new->p_type = p_type;
	(*head) = new;
	return (pointer);
}

void	free_ptr(t_malloc **head, void *addr)
{
	t_malloc	*ptr;
	t_malloc	*previous;

	ptr = *head;
	previous = NULL;
	while (ptr)
	{
		if (ptr->ptr == addr)
		{
			if (previous)
				previous->next = ptr->next;
			else
				*head = ptr->next;
			free(ptr->ptr);
			free(ptr);
			return ;
		}
		previous = ptr;
		ptr = ptr->next;
	}
}
