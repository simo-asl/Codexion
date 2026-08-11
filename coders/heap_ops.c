/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 11:18:24 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/19 14:02:31 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_push(t_heap *heap, t_coder *coder, int edf)
{
	if (heap->size == heap->capacity)
		return (1);
	heap->item[heap->size] = coder;
	heap_up(heap, heap->size, edf);
	heap->size++;
	return (0);
}

void	heap_remove(t_heap *heap, t_coder *coder, int edf)
{
	int	i;

	i = 0;
	while (i < heap->size && heap->item[i] != coder)
		i++;
	if (i == heap->size)
		return ;
	heap->size--;
	if (i == heap->size)
		return ;
	heap->item[i] = heap->item[heap->size];
	if (i > 0 && has_higher_priority(heap->item[i],
			heap->item[(i - 1) / 2], edf))
		heap_up(heap, i, edf);
	else
		heap_down(heap, i, edf);
}
