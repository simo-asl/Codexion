/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 00:00:00 by mel-asla          #+#    #+#             */
/*   Updated: 2026/04/24 04:09:26 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	remove_request_at_index(t_table *table, int index)
{
	t_heap	*heap;

	heap = &table->request_heap;
	if (index < 0 || index >= heap->size)
		return ;
	heap->size--;
	if (index == heap->size)
		return ;
	heap->items[index] = heap->items[heap->size];
	heapify_down(heap, index, table->scheduler);
	heapify_up(heap, index, table->scheduler);
}

static void	update_best_request(t_table *table, int i,
								int *best_index, t_request *best_request)
{
	t_request	current;

	current = table->request_heap.items[i];
	if (*best_index < 0 || compare_requests(current,
			*best_request, table->scheduler) < 0)
	{
		*best_index = i;
		*best_request = current;
	}
}

int	find_best_eligible_request(t_table *table, long now)
{
	int			i;
	int			best_index;
	t_coder		*candidate;
	t_request	best_request;

	best_index = -1;
	i = 0;
	while (i < table->request_heap.size)
	{
		candidate = &table->coders[table->request_heap.items[i].coder_id - 1];
		if (both_dongles_available(candidate, now))
			update_best_request(table, i, &best_index, &best_request);
		i++;
	}
	return (best_index);
}

int	enqueue_request(t_coder *coder)
{
	t_table		*table;
	t_request	req;
	long		last_start;

	table = coder->table;
	if (coder->in_wait_queue)
		return (0);
	pthread_mutex_lock(&table->state_mutex);
	last_start = coder->last_compile_start_ms;
	pthread_mutex_unlock(&table->state_mutex);
	req.coder_id = coder->id;
	req.deadline_ms = last_start + table->time_to_burnout;
	req.sequence = table->next_sequence++;
	if (heap_push(&table->request_heap, req, table->scheduler) != 0)
		return (1);
	coder->in_wait_queue = true;
	return (0);
}
