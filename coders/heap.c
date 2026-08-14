/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:06:00 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/14 13:37:12 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	scheduler_precedes(t_heap *heap, t_coder *a, t_coder *b)
{
	if (heap->is_edf == FIFO)
	{
		if (a->requested_at == b->requested_at)
			return (a->id < b->id);
		return (a->requested_at < b->requested_at);
	}
	else
	{
		if (a->deadline == b->deadline)
		{
			if (a->compile_count == b->compile_count)
				return (a->requested_at < b->requested_at);
			return (a->compile_count < b->compile_count);
		}
		return (a->deadline < b->deadline);
	}
}

void	heap_up(t_heap *heap, int idx)
{
	int		parent;
	t_coder	*tmp;

	while (idx > 0)
	{
		parent = (idx - 1) / 2;
		if (!scheduler_precedes(heap, heap->item[idx], heap->item[parent]))
			break ;
		tmp = heap->item[idx];
		heap->item[idx] = heap->item[parent];
		heap->item[parent] = tmp;
		idx = parent;
	}
}

void	heap_down(t_heap *heap, int idx, int size)
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
		if (left < size && scheduler_precedes(heap,
				heap->item[left], heap->item[best]))
			best = left;
		if (right < size && scheduler_precedes(heap,
				heap->item[right], heap->item[best]))
			best = right;
		if (best == idx)
			break ;
		tmp = heap->item[idx];
		heap->item[idx] = heap->item[best];
		heap->item[best] = tmp;
		idx = best;
	}
}

void	heap_remove_index(t_heap *heap, int index)
{
	if (!heap || index < 0 || index >= heap->size)
		return ;
	heap->size--;
	if (index == heap->size)
		return ;
	heap->item[index] = heap->item[heap->size];
	heap_down(heap, index, heap->size);
}

t_heap	*create_heap(int capacity, int is_edf)
{
	t_heap	*heap;

	if (capacity <= 0)
		return (NULL);
	heap = malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	heap->item = malloc(sizeof(t_coder *) * capacity);
	if (!heap->item)
	{
		free(heap);
		return (NULL);
	}
	heap->capacity = capacity;
	heap->size = 0;
	heap->is_edf = is_edf;
	return (heap);
}
