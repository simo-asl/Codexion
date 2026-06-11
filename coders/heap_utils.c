/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 00:00:00 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/11 09:15:07 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	heap_is_empty(t_queue *queue)
{
	if (!queue)
		return (true);
	return (queue->size == 0);
}

bool	heap_is_full(t_queue *queue)
{
	if (!queue)
		return (true);
	return (queue->size >= queue->max_size);
}

int	heap_find_index(t_queue *queue, t_coder *coder)
{
	int	i;

	if (!queue)
		return (-1);
	i = 0;
	while (i < queue->size)
	{
		if (queue->items[i] == coder)
			return (i);
		i++;
	}
	return (-1);
}

int	heap_insert(t_queue *queue, t_coder *coder)
{
	if (!queue || !coder || heap_is_full(queue))
		return (1);
	queue->items[queue->size] = coder;
	bubble_up(queue, queue->size);
	queue->size++;
	return (0);
}
