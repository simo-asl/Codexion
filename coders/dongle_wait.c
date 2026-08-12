/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_wait.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-asla <mel-asla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 08:44:52 by mel-asla          #+#    #+#             */
/*   Updated: 2026/08/11 19:47:00 by mel-asla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	notify_dongle_waiters(t_dongle *dongle)
{
	dongle->change_version++;
	pthread_cond_broadcast(&dongle->resource_changed);
}

static t_dongle	*cooldown_dongle(t_coder *coder)
{
	if (coder->left->ready_at > coder->right->ready_at)
		return (coder->left);
	return (coder->right);
}

t_dongle	*select_blocking_dongle(t_coder *coder, long long now,
	unsigned long *observed_version, long long *wake_at)
{
	t_dongle	*blocked;

	blocked = coder->left;
	if (coder->left->queue.item[0] != coder)
		blocked = coder->left;
	else if (coder->right->queue.item[0] != coder)
		blocked = coder->right;
	else if (!coder->left->busy && !coder->right->busy
		&& (now < coder->left->ready_at || now < coder->right->ready_at))
		blocked = cooldown_dongle(coder);
	else if (coder->right->busy)
		blocked = coder->right;
	*observed_version = blocked->change_version;
	*wake_at = coder->deadline;
	if (!blocked->busy && blocked->ready_at > now
		&& blocked->ready_at < *wake_at)
		*wake_at = blocked->ready_at;
	return (blocked);
}

void	wait_on_dongle(t_dongle *dongle, unsigned long observed_version,
	long long wake_at)
{
	struct timespec	wake_time;

	wake_time.tv_sec = wake_at / 1000;
	wake_time.tv_nsec = (wake_at % 1000) * 1000000;
	pthread_mutex_lock(&dongle->mutex);
	if (dongle->change_version == observed_version)
		pthread_cond_timedwait(&dongle->resource_changed,
			&dongle->mutex, &wake_time);
	pthread_mutex_unlock(&dongle->mutex);
}
