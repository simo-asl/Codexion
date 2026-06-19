/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 11:05:39 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/19 13:40:55 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_higher_priority(t_queue *queue, t_coder *a, t_coder *b)
{
	if (queue->scheduler == SCH_FIFO)
	{
		if (a->request_timestamp == b->request_timestamp)
			return (a->id < b->id);
		return (a->request_timestamp < b->request_timestamp);
	}
	else
	{
		if (a->burnout_deadline == b->burnout_deadline)
		{
			if (a->completed_compiles == b->completed_compiles)
				return (a->id < b->id);
			return (a->completed_compiles < b->completed_compiles);
		}
		return (a->burnout_deadline < b->burnout_deadline);
	}
}

void	bubble_up(t_queue *queue, int idx)
{
	int		parent;
	t_coder	*tmp;

	while (idx > 0)
	{
		parent = (idx - 1) / 2;
		if (!is_higher_priority(queue, queue->items[idx], queue->items[parent]))
			break ;
		tmp = queue->items[idx];
		queue->items[idx] = queue->items[parent];
		queue->items[parent] = tmp;
		idx = parent;
	}
}

void	bubble_down(t_queue *queue, int idx, int size)
{
	int		left;
	int		right;
	int		best;
	t_coder	*tmp;

	while (1)
	{
		left = 2 * idx + 1;
		right = 2 * idx + 2;
		best = idx;
		if (left < size && is_higher_priority(queue,
				queue->items[left], queue->items[best]))
			best = left;
		if (right < size && is_higher_priority(queue,
				queue->items[right], queue->items[best]))
			best = right;
		if (best == idx)
			break ;
		tmp = queue->items[idx];
		queue->items[idx] = queue->items[best];
		queue->items[best] = tmp;
		idx = best;
	}
}

void	heap_remove_at(t_queue *queue, int index)
{
	if (!queue || index < 0 || index >= queue->size)
		return ;
	queue->size--;
	if (index == queue->size)
		return ;
	queue->items[index] = queue->items[queue->size];
	bubble_down(queue, index, queue->size);
}

t_queue	*heap_init(int max_size, t_scheduler_type scheduler)
{
	t_queue	*queue;

	if (max_size <= 0)
		return (NULL);
	queue = malloc(sizeof(t_queue));
	if (!queue)
		return (NULL);
	queue->items = malloc(sizeof(t_coder *) * max_size);
	if (!queue->items)
	{
		free(queue);
		return (NULL);
	}
	queue->max_size = max_size;
	queue->size = 0;
	queue->scheduler = scheduler;
	return (queue);
}
