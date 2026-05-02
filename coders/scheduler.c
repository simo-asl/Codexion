/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 08:05:52 by mel-asla          #+#    #+#             */
/*   Updated: 2026/05/01 18:46:27 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	deadline_reached(t_coder *coder, long now)
{
	t_table	*table;
	long	deadline;
	int		compiles_done;

	table = coder->table;
	pthread_mutex_lock(&table->state_mutex);
	compiles_done = coder->compiles_done;
	deadline = coder->last_compile_start_ms + table->time_to_burnout;
	pthread_mutex_unlock(&table->state_mutex);
	if (table->required_compiles > 0
		&& compiles_done >= table->required_compiles)
		return (false);
	return (now >= deadline);
}

int	compare_requests(t_request a, t_request b, t_scheduler_type type)
{
	if (type == SCHED_TYPE_FIFO)
	{
		if (a.sequence < b.sequence)
			return (-1);
		if (a.sequence > b.sequence)
			return (1);
		return (0);
	}
	if (a.deadline_ms < b.deadline_ms)
		return (-1);
	if (a.deadline_ms > b.deadline_ms)
		return (1);
	if (a.sequence < b.sequence)
		return (-1);
	if (a.sequence > b.sequence)
		return (1);
	return (0);
}

void	remove_request(t_table *table, int coder_id)
{
	int	i;

	i = 0;
	while (i < table->request_heap.size)
	{
		if (table->request_heap.items[i].coder_id == coder_id)
		{
			table->request_heap.size--;
			table->request_heap.items[i] = table->request_heap.items[
				table->request_heap.size];
			heapify_down(&table->request_heap, i, table->scheduler);
			heapify_up(&table->request_heap, i, table->scheduler);
			return ;
		}
		i++;
	}
}

bool	can_grant_request(t_table *table, t_coder *coder)
{
	int			best_index;
	t_request	selected;
	long		now;

	if (table->request_heap.size <= 0)
		return (false);
	now = get_timestamp_ms();
	best_index = find_best_eligible_request(table, now);
	if (best_index < 0)
		return (false);
	selected = table->request_heap.items[best_index];
	if (selected.coder_id != coder->id
		|| deadline_reached(coder, now)
		|| !reserve_dongles_if_available(coder, now))
		return (false);
	remove_request(table, coder->id);
	coder->in_wait_queue = false;
	pthread_mutex_lock(&table->state_mutex);
	coder->state = STATE_COMPILING;
	coder->last_compile_start_ms = now;
	pthread_mutex_unlock(&table->state_mutex);
	return (true);
}

int	request_dongles(t_coder *coder)
{
	t_table		*table;
	long		delay_ms;
	t_timespec	timeout;

	table = coder->table;
	if (enqueue_request(coder) != 0)
		return (1);
	pthread_cond_broadcast(&table->request_cond);
	while (!get_simulation_end(table))
	{
		if (can_grant_request(table, coder))
			return (0);
		delay_ms = max_ready_time_ms(coder) - get_timestamp_ms();
		if (delay_ms < 0)
			delay_ms = 0;
		build_timeout(delay_ms, &timeout);
		pthread_cond_timedwait(&table->request_cond,
			&table->request_mutex, &timeout);
	}
	if (coder->in_wait_queue)
	{
		remove_request(table, coder->id);
		coder->in_wait_queue = false;
	}
	return (1);
}
