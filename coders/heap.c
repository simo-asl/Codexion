/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 11:05:39 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/19 13:40:55 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	has_higher_priority(t_coder *a, t_coder *b, int edf)
{
	if (edf && a->deadline != b->deadline)
		return (a->deadline < b->deadline);
	if (edf && a->compiled != b->compiled)
		return (a->compiled < b->compiled);
	if (a->ticket != b->ticket)
		return (a->ticket < b->ticket);
	return (a->id < b->id);
}

static void	swap(t_coder **a, t_coder **b)
{
	t_coder	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	heap_up(t_heap *heap, int i, int edf)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!has_higher_priority(heap->item[i], heap->item[parent], edf))
			break ;
		swap(&heap->item[i], &heap->item[parent]);
		i = parent;
	}
}

void	heap_down(t_heap *heap, int i, int edf)
{
	int	left;
	int	best;

	while (i * 2 + 1 < heap->size)
	{
		left = i * 2 + 1;
		best = left;
		if (left + 1 < heap->size
			&& has_higher_priority(heap->item[left + 1], heap->item[left], edf))
			best = left + 1;
		if (!has_higher_priority(heap->item[best], heap->item[i], edf))
			break ;
		swap(&heap->item[i], &heap->item[best]);
		i = best;
	}
}
