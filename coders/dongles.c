/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 07:58:11 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/11 20:56:00 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	queue_request(t_coder *coder)
{
	lock_dongle_pair(coder);
	pthread_mutex_lock(&coder->sim->state);
	coder->ticket = coder->sim->next_ticket++;
	pthread_mutex_unlock(&coder->sim->state);
	if (heap_push(&coder->left->queue, coder, coder->sim->cfg.edf)
		|| heap_push(&coder->right->queue, coder, coder->sim->cfg.edf))
	{
		heap_remove(&coder->left->queue, coder, coder->sim->cfg.edf);
		unlock_dongle_pair(coder);
		return (1);
	}
	notify_dongle_waiters(coder->left);
	notify_dongle_waiters(coder->right);
	unlock_dongle_pair(coder);
	return (0);
}

static int	can_take(t_coder *coder, long long now)
{
	return (coder->left->queue.item[0] == coder
		&& coder->right->queue.item[0] == coder && !coder->left->busy
		&& !coder->right->busy && now >= coder->left->ready_at
		&& now >= coder->right->ready_at);
}

static int	claim(t_coder *coder, long long now)
{
	int	claimed;

	claimed = 0;
	pthread_mutex_lock(&coder->sim->state);
	if (!coder->sim->stop && now < coder->deadline)
	{
		coder->compile_start = now;
		coder->deadline = now + coder->sim->cfg.burnout;
		coder->left->busy = 1;
		coder->right->busy = 1;
		claimed = 1;
	}
	pthread_mutex_unlock(&coder->sim->state);
	return (claimed);
}

static int	try_take(t_coder *coder, t_dongle **blocked,
	unsigned long *observed_version, long long *wake_at)
{
	long long	now;
	int			claimed;

	claimed = 0;
	lock_dongle_pair(coder);
	now = current_time_ms();
	if (can_take(coder, now))
		claimed = claim(coder, now);
	if (claimed)
	{
		heap_remove(&coder->left->queue, coder, coder->sim->cfg.edf);
		heap_remove(&coder->right->queue, coder, coder->sim->cfg.edf);
		notify_dongle_waiters(coder->left);
		notify_dongle_waiters(coder->right);
	}
	else
		*blocked = select_blocking_dongle(coder, now,
				observed_version, wake_at);
	unlock_dongle_pair(coder);
	return (claimed);
}

int	request_dongles(t_coder *coder)
{
	t_dongle		*blocked;
	unsigned long	observed_version;
	long long		wake_at;

	if (queue_request(coder))
		return (1);
	while (!simulation_stopped(coder->sim))
	{
		if (try_take(coder, &blocked, &observed_version, &wake_at))
		{
			print_coder_state(coder, "has taken a dongle");
			print_coder_state(coder, "has taken a dongle");
			print_coder_state(coder, "is compiling");
			return (0);
		}
		wait_on_dongle(blocked, observed_version, wake_at);
	}
	leave_queues(coder);
	return (1);
}
