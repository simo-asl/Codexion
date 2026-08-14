/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 18:32:05 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/14 13:37:12 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	enqueue_request(t_coder *coder, t_dongle *first,
	t_dongle *second)
{
	lock_dongles(first, second, 1);
	if (heap_push(coder->left->queue, coder) != SUCCESS)
	{
		unlock_dongles(first, second, 1);
		return (FAIL);
	}
	if (heap_push(coder->right->queue, coder) != SUCCESS)
	{
		heap_remove_index(coder->left->queue,
			heap_find_index(coder->left->queue, coder));
		unlock_dongles(first, second, 1);
		return (FAIL);
	}
	unlock_dongles(first, second, 1);
	return (SUCCESS);
}

static int	try_claim_pair(t_coder *coder, t_dongle *first, t_dongle *second)
{
	long long	now;

	lock_dongles(first, second, 1);
	now = current_time_ms();
	if (!is_queue_head(coder->left, coder)
		|| !is_queue_head(coder->right, coder)
		|| first->busy || second->busy
		|| now < first->ready_at || now < second->ready_at)
	{
		unlock_dongles(first, second, 1);
		usleep(50);
		return (FAIL);
	}
	first->busy = 1;
	second->busy = 1;
	remove_from_queues(coder);
	unlock_dongles(first, second, 1);
	print_coder_state(coder, "has taken a dongle");
	print_coder_state(coder, "has taken a dongle");
	return (SUCCESS);
}

int	request_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	order_dongles(coder, &first, &second);
	coder->requested_at = current_time_ms();
	if (enqueue_request(coder, first, second) == FAIL)
		return (FAIL);
	while (!simulation_stopped(coder->sim))
	{
		if (try_claim_pair(coder, first, second) == SUCCESS)
			return (SUCCESS);
	}
	lock_dongles(first, second, 1);
	remove_from_queues(coder);
	unlock_dongles(first, second, 1);
	return (FAIL);
}

void	release_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	long long	now;
	long long	cd;

	cd = coder->sim->config.dongle_cooldown;
	order_dongles(coder, &first, &second);
	lock_dongles(first, second, 0);
	now = current_time_ms();
	first->busy = 0;
	first->ready_at = now + cd;
	second->busy = 0;
	second->ready_at = now + cd;
	unlock_dongles(first, second, 0);
}
