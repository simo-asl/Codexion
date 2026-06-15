/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 07:58:11 by mel-asla          #+#    #+#             */
/*   Updated: 2026/06/15 14:15:13 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	insert_in_heaps(t_coder *coder, t_dongle *first, t_dongle *second)
{
	lock_dongles(first, second, 1);
	if (heap_insert(coder->left_dongle->queue, coder) != 0)
	{
		unlock_dongles(first, second, 1);
		return (1);
	}
	if (heap_insert(coder->right_dongle->queue, coder) != 0)
	{
		heap_remove_at(coder->left_dongle->queue,
			heap_find_index(coder->left_dongle->queue, coder));
		unlock_dongles(first, second, 1);
		return (1);
	}
	unlock_dongles(first, second, 1);
	return (0);
}

static int	try_take(t_coder *coder, t_dongle *first, t_dongle *second)
{
	long long	now;

	lock_dongles(first, second, 1);
	now = get_time_in_ms();
	if (!is_top_of_heap(coder->left_dongle, coder)
		|| !is_top_of_heap(coder->right_dongle, coder)
		|| first->in_use || second->in_use
		|| now < first->cooldown || now < second->cooldown)
	{
		unlock_dongles(first, second, 1);
		usleep(50);
		return (1);
	}
	first->in_use = 1;
	second->in_use = 1;
	cleanup_heaps_locked(coder);
	unlock_dongles(first, second, 1);
	log_event(coder, "has taken a dongle");
	log_event(coder, "has taken a dongle");
	return (0);
}

int	acquire_dongles(t_coder *coder)
{
	t_dongle	*first_d;
	t_dongle	*second_d;

	lock_order(coder, &first_d, &second_d);
	coder->request_timestamp = get_time_in_ms();
	if (insert_in_heaps(coder, first_d, second_d) != 0)
		return (1);
	while (!need_to_stop(coder->sim))
	{
		if (try_take(coder, first_d, second_d) == 0)
			return (0);
	}
	lock_dongles(first_d, second_d, 1);
	cleanup_heaps_locked(coder);
	unlock_dongles(first_d, second_d, 1);
	return (1);
}

void	release_dongles(t_coder *coder)
{
	t_dongle		*first;
	t_dongle		*second;
	long long		now;
	long long		cooldown;

	cooldown = coder->sim->config.dongle_cooldown;
	lock_order(coder, &first, &second);
	lock_dongles(first, second, 0);
	now = get_time_in_ms();
	first->in_use = 0;
	first->cooldown = now + cooldown;
	second->in_use = 0;
	second->cooldown = now + cooldown;
	unlock_dongles(first, second, 0);
}
