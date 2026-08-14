/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 10:00:00 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/14 13:37:12 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_is_empty(t_heap *heap)
{
	if (!heap)
		return (1);
	return (heap->size == 0);
}

int	heap_is_full(t_heap *heap)
{
	if (!heap)
		return (1);
	return (heap->size >= heap->capacity);
}

int	heap_find_index(t_heap *heap, t_coder *coder)
{
	int	i;

	if (!heap)
		return (-1);
	i = 0;
	while (i < heap->size)
	{
		if (heap->item[i] == coder)
			return (i);
		i++;
	}
	return (-1);
}

int	heap_push(t_heap *heap, t_coder *coder)
{
	if (!heap || !coder || heap_is_full(heap))
		return (FAIL);
	heap->item[heap->size] = coder;
	heap_up(heap, heap->size);
	heap->size++;
	return (SUCCESS);
}
